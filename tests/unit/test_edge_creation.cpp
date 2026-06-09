#include "GridGenerator.h"
#include "GridSpec.h"

#include <iostream>
#include <unordered_set>

int main()
{
    // ------------------------------------------------------------
    // 1. Generate grid
    // ------------------------------------------------------------
    GridSpec spec = GridSpec::Minimal(10);

    GridGenerator gen;
    Grid grid = gen.generate(spec);

    const auto& nodes = grid.nodes();
    const auto& edges = grid.edges();

    // ------------------------------------------------------------
    // 2. Basic sanity checks
    // ------------------------------------------------------------
    if (nodes.size() != spec.num_nodes) {
        std::cerr << "Node count mismatch\n";
        return 1;
    }

    if (edges.empty()) {
        std::cerr << "Expected at least one edge\n";
        return 1;
    }

    // ------------------------------------------------------------
    // 3. Validate edge index safety
    // ------------------------------------------------------------
    std::unordered_set<NodeID> valid_nodes;
    for (const auto& n : nodes) {
        valid_nodes.insert(n.id);
    }

    for (const auto& e : edges) {
        if (!valid_nodes.count(e.from)) {
            std::cerr << "Invalid edge source: " << e.from << "\n";
            return 1;
        }

        if (!valid_nodes.count(e.to)) {
            std::cerr << "Invalid edge target: " << e.to << "\n";
            return 1;
        }

        if (e.from == e.to) {
            std::cerr << "Self-loop detected at node " << e.from << "\n";
            return 1;
        }
    }

    // ------------------------------------------------------------
    // 4. Connectivity sanity (weak structural check)
    // ------------------------------------------------------------
    // Ensure every node participates in at least one edge
    std::unordered_set<NodeID> connected;

    for (const auto& e : edges) {
        connected.insert(e.from);
        connected.insert(e.to);
    }

    for (const auto& n : nodes) {
        if (!connected.count(n.id)) {
            std::cerr << "Isolated node detected: " << n.id << "\n";
            return 1;
        }
    }

    // ------------------------------------------------------------
    // 5. Minimum spanning density check
    // ------------------------------------------------------------
    if (edges.size() < nodes.size() - 1) {
        std::cerr << "Graph too sparse for a connected backbone: "
                  << edges.size()
                  << " < "
                  << (nodes.size() - 1)
                  << "\n";
        return 1;
    }

    std::cout << "Edge structural validity test passed.\n";
    return 0;
}
