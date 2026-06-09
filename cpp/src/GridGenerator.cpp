#include "GridGenerator.h"
#include "GridBuilder.h"
#include <queue>
#include <stdexcept>
#include <algorithm>

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------
GridGenerator::GridGenerator(uint64_t seed)
    : m_random_engine(seed) // seed RNG for deterministic generation
{
    // Nothing else to do; the generator is stateless
}

// ------------------------------------------------------------
// GridSpec validity checker
// ------------------------------------------------------------
bool GridGenerator::spec_check(const GridSpec& spec,
                               std::string& error) const
{
    if (spec.num_nodes == 0) {
        error = "num_nodes must be greater than zero";
        return false;
    }

    if (spec.generator_ratio < 0.0 || spec.generator_ratio > 1.0) {
        error = "generator_ratio must be in [0,1]";
        return false;
    }

    if (spec.load_ratio < 0.0 || spec.load_ratio > 1.0) {
        error = "load_ratio must be in [0,1]";
        return false;
    }

    const size_t generators =
        static_cast<size_t>(spec.generator_ratio * spec.num_nodes);

    const size_t loads =
        static_cast<size_t>(spec.load_ratio * spec.num_nodes);

    if (spec.num_nodes >= 1 && generators == 0) {
        error =
            "generator_ratio yields zero generators; "
            "at least one generator is required";
        return false;
    }

    if (spec.num_nodes > 1 && loads == 0) {
        error =
            "load_ratio yields zero loads; "
            "at least one load is required when num_nodes > 1";
        return false;
    }

    if (generators + loads > spec.num_nodes) {
        error =
            "generator and load counts exceed available nodes";
        return false;
    }

    return true;
}

// ============================================================
// GridGenerator::generate
// ============================================================
Grid GridGenerator::generate(const GridSpec& spec)
{
    const size_t n = spec.num_nodes;
    if (n == 0)
        throw std::invalid_argument("GridSpec::num_nodes must be > 0");

    GridBuilder builder(n);

    // -------------------------------
    // 1. Build backbone (chain or ring)
    // -------------------------------
    for (NodeID i = 0; i + 1 < n; ++i) {
        builder.add_edge(i, i + 1);
    }
    if (spec.use_ring_backbone && n > 2) {
        builder.add_edge(n - 1, 0);
    }

    // -------------------------------
    // 2. Branching expansion
    // -------------------------------
    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

    for (NodeID parent = 0; parent < n; ++parent) {
        size_t num_children = std::max<size_t>(0,
            static_cast<size_t>(std::round(
                prob_dist(m_random_engine) * 2.0 * spec.branch_factor)));

        for (size_t c = 0; c < num_children; ++c) {
            NodeID child = static_cast<NodeID>(m_random_engine() % n);
            if (child != parent) {
                builder.add_edge(parent, child);
            }
        }
    }

    // -------------------------------
    // 3. Optional cross-links
    // -------------------------------
    if (spec.add_cross_links) {
        for (NodeID i = 0; i < n; ++i) {
            for (NodeID j = i + 1; j < n; ++j) {
                if (prob_dist(m_random_engine) < spec.cross_link_rate) {
                    builder.add_edge(i, j);
                }
            }
        }
    }

    // -------------------------------
    // 4. Temporary adjacency for role assignment & repair
    // -------------------------------
    std::vector<std::vector<NodeID>> adj(n);
    for (const auto& e : builder.edges()) {  // <-- need builder.edges() accessor
        adj[e.from].push_back(e.to);
        adj[e.to].push_back(e.from);
    }

    // -------------------------------
    // 5. Node type assignment
    // -------------------------------
    std::vector<NodeID> nodes(n);
    std::iota(nodes.begin(), nodes.end(), 0);

    std::vector<size_t> degree(n, 0);
    for (NodeID i = 0; i < n; ++i) {
        degree[i] = adj[i].size();
    }

    std::vector<NodeType> type(n, NodeType::Bus);

    // Generators → hubs
    size_t num_generators = std::max<size_t>(1, static_cast<size_t>(spec.generator_ratio * n));
    std::sort(nodes.begin(), nodes.end(),
              [&](NodeID a, NodeID b) { return degree[a] > degree[b]; });
    for (size_t i = 0; i < num_generators && i < n; ++i) {
        type[nodes[i]] = NodeType::Generator;
    }

    // Loads → leaves
    size_t num_loads = std::max<size_t>(1, static_cast<size_t>(spec.load_ratio * n));
    std::sort(nodes.begin(), nodes.end(),
              [&](NodeID a, NodeID b) { return degree[a] < degree[b]; });
    size_t assigned_loads = 0;
    for (size_t i = 0; i < n && assigned_loads < num_loads; ++i) {
        NodeID v = nodes[i];
        if (type[v] == NodeType::Generator) continue;
        type[v] = NodeType::Load;
        assigned_loads++;
    }

    // Commit node types to builder
    for (NodeID i = 0; i < n; ++i) {
        builder.set_node_type(i, type[i]);
    }

    // -------------------------------
    // 6. Connectivity repair
    // -------------------------------
    if (spec.require_connected_graph && num_generators > 0) {
        NodeID root = 0;
        std::vector<bool> connected(n, false);
        connected[root] = true;

        std::queue<NodeID> q;
        q.push(root);

        while (!q.empty()) {
            NodeID u = q.front(); q.pop();
            for (NodeID v : adj[u]) {
                if (!connected[v]) {
                    connected[v] = true;
                    q.push(v);
                }
            }
        }

        for (NodeID i = 0; i < n; ++i) {
            if (!connected[i] && type[i] != NodeType::Generator) {
                builder.add_edge(root, i);
            }
        }
    }

    // -------------------------------
    // 7. Generator reachability repair
    // -------------------------------
    if (spec.require_generator_reachability && num_generators > 0) {
        std::vector<bool> reachable(n, false);
        std::queue<NodeID> q;

        for (NodeID i = 0; i < n; ++i) {
            if (type[i] == NodeType::Generator) {
                reachable[i] = true;
                q.push(i);
            }
        }

        while (!q.empty()) {
            NodeID u = q.front(); q.pop();
            for (NodeID v : adj[u]) {
                if (!reachable[v]) {
                    reachable[v] = true;
                    q.push(v);
                }
            }
        }

        for (NodeID i = 0; i < n; ++i) {
            if (type[i] == NodeType::Load && !reachable[i]) {
                builder.add_edge(0, i); // repair path
            }
        }
    }

    // -------------------------------
    // 8. Freeze and return
    // -------------------------------
    return std::move(builder).build();
}
// Grid GridGenerator::generate(const GridSpec& spec)
// {
//     const size_t n = spec.num_nodes;
//     if (n == 0)
//         throw std::invalid_argument("GridSpec::num_nodes must be > 0");

//     GridBuilder builder(n);

//     // -------------------------------
//     // 1. Build backbone (chain or ring)
//     // -------------------------------
//     for (NodeID i = 0; i + 1 < n; ++i) {
//         builder.add_edge(i, i + 1);
//     }
//     if (spec.use_ring_backbone && n > 2) {
//         builder.add_edge(n - 1, 0);
//     }

//     // -------------------------------
//     // 2. Branching expansion
//     // -------------------------------
//     std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

//     for (NodeID parent = 0; parent < n; ++parent) {
//         size_t num_children = std::max<size_t>(0,
//             static_cast<size_t>(std::round(
//                 prob_dist(m_random_engine) * 2.0 * spec.branch_factor)));

//         for (size_t c = 0; c < num_children; ++c) {
//             NodeID child = static_cast<NodeID>(
//                 m_random_engine() % n);
//             if (child != parent) {
//                 builder.add_edge(parent, child);
//             }
//         }
//     }

//     // -------------------------------
//     // 3. Optional cross-links
//     // -------------------------------
//     if (spec.add_cross_links) {
//         for (NodeID i = 0; i < n; ++i) {
//             for (NodeID j = i + 1; j < n; ++j) {
//                 if (prob_dist(m_random_engine) < spec.cross_link_rate) {
//                     builder.add_edge(i, j);
//                 }
//             }
//         }
//     }

//     // -------------------------------
//     // 4. Build adjacency for role assignment and repair
//     // -------------------------------
//     std::vector<std::vector<NodeID>> adj(n);
//     for (const auto& e : builder.edges()) {
//         adj[e.from].push_back(e.to);
//         adj[e.to].push_back(e.from); // undirected for degree
//     }

//     // -------------------------------
//     // 5. Topology-aware role assignment
//     // -------------------------------
//     std::vector<NodeID> nodes(n);
//     std::iota(nodes.begin(), nodes.end(), 0);

//     // compute degrees
//     std::vector<size_t> degree(n, 0);
//     for (NodeID u = 0; u < n; ++u) {
//         degree[u] = adj[u].size();
//     }

//     // Assign generators → hubs
//     size_t num_generators =
//         std::max<size_t>(1, static_cast<size_t>(std::round(spec.generator_ratio * n)));

//     std::vector<NodeType> type(n, NodeType::Bus);

//     std::sort(nodes.begin(), nodes.end(),
//         [&](NodeID a, NodeID b) { return degree[a] > degree[b]; });

//     for (size_t i = 0; i < num_generators && i < n; ++i) {
//         type[nodes[i]] = NodeType::Generator;
//     }

//     // Assign loads → leaves
//     size_t num_loads =
//         std::max<size_t>(1, static_cast<size_t>(std::round(spec.load_ratio * n)));

//     std::sort(nodes.begin(), nodes.end(),
//         [&](NodeID a, NodeID b) { return degree[a] < degree[b]; });

//     size_t assigned_loads = 0;
//     for (size_t i = 0; i < n && assigned_loads < num_loads; ++i) {
//         NodeID v = nodes[i];
//         if (type[v] == NodeType::Generator) continue;
//         type[v] = NodeType::Load;
//         assigned_loads++;
//     }

//     // Remaining nodes = buses
//     for (NodeID i = 0; i < n; ++i) {
//         if (type[i] != NodeType::Generator && type[i] != NodeType::Load) {
//             type[i] = NodeType::Bus;
//         }
//     }

//     // commit node types
//     for (NodeID i = 0; i < n; ++i) {
//         builder.set_node_type(i, type[i]);
//     }

//     // -------------------------------
//     // 6. Connectivity enforcement (optional)
//     // -------------------------------
//     if (spec.require_connected_graph && num_generators > 0) {
//         NodeID root = 0;
//         std::vector<bool> connected(n, false);
//         connected[root] = true;

//         std::queue<NodeID> q;
//         q.push(root);

//         while (!q.empty()) {
//             NodeID u = q.front(); q.pop();
//             for (NodeID v : adj[u]) {
//                 if (!connected[v]) {
//                     connected[v] = true;
//                     q.push(v);
//                 }
//             }
//         }

//         for (NodeID i = 0; i < n; ++i) {
//             if (!connected[i] && type[i] != NodeType::Generator) {
//                 builder.add_edge(root, i);
//             }
//         }
//     }

//     // -------------------------------
//     // 7. Generator reachability repair (optional)
//     // -------------------------------
//     if (spec.require_generator_reachability && num_generators > 0) {
//         std::vector<bool> reachable(n, false);
//         std::queue<NodeID> q;
//         for (NodeID i = 0; i < n; ++i) {
//             if (type[i] == NodeType::Generator) {
//                 reachable[i] = true;
//                 q.push(i);
//             }
//         }

//         while (!q.empty()) {
//             NodeID u = q.front(); q.pop();
//             for (NodeID v : adj[u]) {
//                 if (!reachable[v]) {
//                     reachable[v] = true;
//                     q.push(v);
//                 }
//             }
//         }

//         for (NodeID i = 0; i < n; ++i) {
//             if (type[i] == NodeType::Load && !reachable[i]) {
//                 builder.add_edge(0, i); // repair path
//             }
//         }
//     }

//     // -------------------------------
//     // 8. Return final Grid
//     // -------------------------------
//     return std::move(builder).build();
// }