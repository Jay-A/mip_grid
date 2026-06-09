#include "GridValidator.h"
#include <queue>
#include <vector>
#include <stdexcept>

/**
 * @brief Construct validator with fixed rules.
 *
 * @param rules ValidationRules defining feasibility constraints.
 */
GridValidator::GridValidator(const ValidationRules& rules)
    : rules_(rules)
{}

/**
 * @brief Check whether a grid satisfies configured validation rules.
 *
 * @param grid Immutable Grid to evaluate.
 * @return true if grid passes all rules, false otherwise.
 */
bool GridValidator::is_valid(const Grid& grid) const
{
    if (rules_.require_connected && !is_connected(grid)) {
        return false;
    }

    if (rules_.max_unserved_load_fraction < 1.0) {
        double frac = unserved_load_fraction(grid);
        if (frac > rules_.max_unserved_load_fraction) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Check if the grid is fully connected.
 *
 * Performs BFS traversal from node 0 and counts reachable nodes.
 *
 * @param grid Immutable Grid to evaluate.
 * @return true if all nodes are reachable from a single node.
 */
bool GridValidator::is_connected(const Grid& grid) const
{
    const size_t N = grid.num_nodes();

    if (N == 0) {
        return true;
    }

    const auto& adj = grid.adjacency();
    std::vector<bool> visited(N, false);
    std::queue<NodeID> q;

    NodeID start = 0;
    visited[start] = true;
    q.push(start);

    size_t visited_count = 1;

    while (!q.empty()) {
        NodeID cur = q.front();
        q.pop();

        for (NodeID nb : adj[cur]) {
            if (!visited[nb]) {
                visited[nb] = true;
                q.push(nb);
                ++visited_count;
            }
        }
    }

    return visited_count == N;
}

/**
 * @brief Compute fraction of loads not reachable from any generator.
 *
 * Multi-source BFS from all generator nodes using ID-based access.
 *
 * @param grid Immutable Grid to evaluate.
 * @return Fraction of unreachable loads in range [0.0, 1.0].
 */
double GridValidator::unserved_load_fraction(const Grid& grid) const
{
    const size_t N = grid.num_nodes();
    const auto& adj = grid.adjacency();

    if (N == 0) {
        return 0.0;
    }

    std::vector<bool> visited(N, false);
    std::queue<NodeID> q;

    // -----------------------------
    // Multi-source BFS from generators
    // -----------------------------
    for (NodeID i = 0; i < N; ++i) {
        if (grid.node_type(i) == NodeType::Generator) {
            visited[i] = true;
            q.push(i);
        }
    }

    while (!q.empty()) {
        NodeID cur = q.front();
        q.pop();

        for (NodeID nb : adj[cur]) {
            if (!visited[nb]) {
                visited[nb] = true;
                q.push(nb);
            }
        }
    }

    // -----------------------------
    // Count unreachable loads
    // -----------------------------
    size_t total_loads = 0;
    size_t unreachable = 0;

    for (NodeID i = 0; i < N; ++i) {
        if (grid.node_type(i) == NodeType::Load) {
            ++total_loads;
            if (!visited[i]) {
                ++unreachable;
            }
        }
    }

    return total_loads == 0 ? 0.0
                            : static_cast<double>(unreachable) /
                              static_cast<double>(total_loads);
}