#include "GridBuilder.h"
#include <stdexcept>

/**
 * @file GridBuilder.cpp
 * @brief Implementation of the mutable construction interface for immutable Grid.
 *
 * @ingroup grid_builder
 *
 * This source file implements all GridBuilder methods including:
 * - Node and edge construction
 * - Node type assignment
 * - Validation
 * - Move-only build finalization
 *
 * All access to Grid internals is encapsulated and guarded with runtime checks.
 */

// ============================================================
// Constructor
// ============================================================

GridBuilder::GridBuilder(NodeID n)
{
    if (n > 0) {
        // Preallocate vectors for efficiency
        m_grid.m_nodes.reserve(n);
        m_grid.m_node_type.reserve(n);

        // Initialize nodes with sequential IDs and default type Bus
        for (NodeID i = 0; i < n; ++i) {
            m_grid.m_nodes.push_back(Node{ i });
            m_grid.m_node_type.push_back(NodeType::Bus);
        }
    }
}

// ============================================================
// Add node
// ============================================================

NodeID GridBuilder::add_node()
{
    NodeID id = static_cast<NodeID>(m_grid.m_nodes.size());
    m_grid.m_nodes.push_back(Node{ id });
    m_grid.m_node_type.push_back(NodeType::Bus);
    return id;
}

// ============================================================
// Set node type
// ============================================================

void GridBuilder::set_node_type(NodeID id, NodeType type)
{
    if (id >= m_grid.m_nodes.size()) {
        throw std::out_of_range("GridBuilder::set_node_type invalid NodeID");
    }
    m_grid.m_node_type[id] = type;
}

// ============================================================
// Add edge
// ============================================================

void GridBuilder::add_edge(NodeID from, NodeID to)
{
    if (from >= m_grid.m_nodes.size() || to >= m_grid.m_nodes.size()) {
        throw std::out_of_range("GridBuilder::add_edge invalid NodeID");
    }
    m_grid.m_edges.push_back(Edge{ from, to });
}

// ============================================================
// Read-only inspection
// ============================================================

NodeID GridBuilder::num_nodes() const noexcept
{
    return static_cast<NodeID>(m_grid.m_nodes.size());
}

NodeID GridBuilder::num_edges() const noexcept
{
    return static_cast<NodeID>(m_grid.m_edges.size());
}

NodeType GridBuilder::node_type(NodeID id) const
{
    if (id >= m_grid.m_nodes.size()) {
        throw std::out_of_range("GridBuilder::node_type invalid NodeID");
    }
    return m_grid.m_node_type[id];
}

bool GridBuilder::has_node(NodeID id) const noexcept
{
    return id < m_grid.m_nodes.size();
}

// ============================================================
// Read-only accessors for edges (builder-safe)
// ============================================================

const std::vector<Edge>& GridBuilder::edges() const noexcept
{
    return m_grid.m_edges;
}

// Optional: read-only access to node types
const std::vector<NodeType>& GridBuilder::node_types() const noexcept
{
    return m_grid.m_node_type;
}

// ============================================================
// Build final immutable grid (rvalue-qualified)
// ============================================================

Grid GridBuilder::build() &&
{
    // Optional: validate before build
    validate();

    // Transfer ownership
    Grid result = std::move(m_grid);

    // Clear moved-from builder to prevent accidental reuse
    m_grid = Grid();

    return result;
}

// ============================================================
// Internal validation helper
// ============================================================

void GridBuilder::validate() const
{
    // All edges must reference valid nodes
    for (const auto& e : m_grid.m_edges) {
        if (e.from >= m_grid.m_nodes.size() || e.to >= m_grid.m_nodes.size()) {
            throw std::runtime_error("GridBuilder::validate found edge with invalid NodeID");
        }
    }

    // All nodes must have a type
    if (m_grid.m_nodes.size() != m_grid.m_node_type.size()) {
        throw std::runtime_error("GridBuilder::validate found node/type size mismatch");
    }
}