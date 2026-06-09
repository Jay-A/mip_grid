#include "Grid.h"
#include <stdexcept>

const std::vector<std::vector<NodeID>>& Grid::adjacency() const
{
    build_adjacency();
    return m_adjacency;
}

void Grid::build_adjacency() const
{
    if (m_adjacency_valid) return;

    m_adjacency.assign(m_nodes.size(), {});

    for (const auto& e : m_edges)
    {
        if (e.from >= m_nodes.size() || e.to >= m_nodes.size()) {
            throw std::out_of_range(
                "Invalid edge endpoint in Grid::build_adjacency"
            );
        }
        m_adjacency[e.from].push_back(e.to);
    }

    m_adjacency_valid = true;
}