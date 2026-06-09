#include "Grid.h"
#include "GridGenerator.h"
#include "GridSpec.h"

#include <vector>
#include <queue>
#include <algorithm>
#include <stdexcept>
#include <iostream>

// ============================================================
// Feasibility function (kept reusable)
// ============================================================

bool check_grid_feasible(const Grid& grid)
{
    const size_t n = grid.num_nodes();
    if (n == 0) {
        std::cerr << "Grid has no nodes\n";
        return false;
    }

    const size_t m = grid.num_edges();
    if (m == 0) {
        std::cerr << "Grid has no edges\n";
        return false;
    }

    // Count generators and loads
    size_t num_generators = 0;
    size_t num_loads = 0;

    for (NodeID i = 0; i < n; ++i) {
        NodeType t = grid.node_type(i);
        if (t == NodeType::Generator) num_generators++;
        if (t == NodeType::Load) num_loads++;
    }

    if (num_generators == 0) {
        std::cerr << "No generators in grid\n";
        return false;
    }

    if (num_loads == 0 && n > 1) {
        std::cerr << "No loads in grid (num_nodes > 1)\n";
        return false;
    }

    // Build adjacency list
    std::vector<std::vector<NodeID>> adj(n);

    for (EdgeID i = 0; i < m; ++i) {
        const Edge& e = grid.edge(i);

        if (e.from >= n || e.to >= n) {
            std::cerr << "Invalid edge from " << e.from
                      << " to " << e.to << "\n";
            return false;
        }

        adj[e.from].push_back(e.to);
        adj[e.to].push_back(e.from);
    }

    // Connectivity BFS
    std::vector<bool> visited(n, false);
    std::queue<NodeID> q;

    q.push(0);
    visited[0] = true;

    while (!q.empty()) {
        NodeID u = q.front();
        q.pop();

        for (NodeID v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    for (NodeID i = 0; i < n; ++i) {
        if (!visited[i] && grid.node_type(i) != NodeType::Generator) {
            std::cerr << "Node " << i << " is disconnected\n";
            return false;
        }
    }

    // Generator reachability
    std::vector<bool> reachable(n, false);
    std::queue<NodeID> gen_queue;

    for (NodeID i = 0; i < n; ++i) {
        if (grid.node_type(i) == NodeType::Generator) {
            reachable[i] = true;
            gen_queue.push(i);
        }
    }

    while (!gen_queue.empty()) {
        NodeID u = gen_queue.front();
        gen_queue.pop();

        for (NodeID v : adj[u]) {
            if (!reachable[v]) {
                reachable[v] = true;
                gen_queue.push(v);
            }
        }
    }

    for (NodeID i = 0; i < n; ++i) {
        if (grid.node_type(i) == NodeType::Load && !reachable[i]) {
            std::cerr << "Load node " << i
                      << " cannot reach any generator\n";
            return false;
        }
    }

    return true;
}

// ============================================================
// TEST MAIN (THIS WAS MISSING)
// ============================================================

int main()
{
    GridGenerator gen;
    GridSpec spec = GridSpec::Minimal(10);

    Grid grid = gen.generate(spec);

    if (check_grid_feasible(grid)) {
        std::cout << "Grid feasibility test passed.\n";
        return 0;
    }

    std::cout << "Grid feasibility test FAILED.\n";
    return 1;
}