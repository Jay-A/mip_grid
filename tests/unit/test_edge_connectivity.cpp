#include "GridGenerator.h"

#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>

int main()
{
    // ------------------------------------------------------------
    // 1. Generate grid (correct API)
    // ------------------------------------------------------------
    GridSpec spec = GridSpec::Minimal(10);

    GridGenerator gen;
    Grid grid = gen.generate(spec);

    const auto& nodes = grid.nodes();  
    const auto& edges = grid.edges();  

    // ------------------------------------------------------------
    // 2. Basic sanity
    // ------------------------------------------------------------
    if (nodes.empty()) {
        std::cerr << "No nodes generated\n";
        return 1;
    }

    if (edges.empty()) {
        std::cerr << "No edges generated\n";
        return 1;
    }

    // ------------------------------------------------------------
    // 3. Index validity (still useful)
    // ------------------------------------------------------------
    std::unordered_set<NodeID> valid;
    for (const auto& n : nodes) {
        valid.insert(n.id);
    }

    for (const auto& e : edges) {
        if (!valid.count(e.from)) {
            std::cerr << "Invalid edge source: " << e.from << "\n";
            return 1;
        }

        if (!valid.count(e.to)) {
            std::cerr << "Invalid edge target: " << e.to << "\n";
            return 1;
        }

        if (e.from == e.to) {
            std::cerr << "Self-loop detected at " << e.from << "\n";
            return 1;
        }
    }

    // ------------------------------------------------------------
    // 4. REAL connectivity test (BFS/DFS)
    // ------------------------------------------------------------
    std::unordered_set<NodeID> visited;
    std::vector<NodeID> stack;

    stack.push_back(nodes.front().id);
    visited.insert(nodes.front().id);

    std::unordered_map<NodeID, std::vector<NodeID>> adj;

    for (const auto& e : edges) {
        adj[e.from].push_back(e.to);
        adj[e.to].push_back(e.from);
    }

    while (!stack.empty()) {
        NodeID cur = stack.back();
        stack.pop_back();

        for (NodeID nb : adj[cur]) {
            if (!visited.count(nb)) {
                visited.insert(nb);
                stack.push_back(nb);
            }
        }
    }

    // ------------------------------------------------------------
    // 5. Ensure full graph connectivity
    // ------------------------------------------------------------
    if (visited.size() != nodes.size()) {
        std::cerr << "Graph is not fully connected: "
                  << visited.size()
                  << " / "
                  << nodes.size()
                  << "\n";
        return 1;
    }

    std::cout << "Edge connectivity test passed.\n";
    return 0;
}
