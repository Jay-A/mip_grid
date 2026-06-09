#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>
#include <stdexcept>

#include "Node.h"
#include "Edge.h"
#include "Types.h"

/**
 * @file Grid.h
 * @brief Immutable graph container for synthetic power grid topology.
 *
 * @defgroup grid_module Grid
 * @brief Core topology representation for synthetic power grids.
 *
 * The Grid class encapsulates the nodes, edges, and node types of a synthetic power
 * network. It is designed to be immutable after construction by GridBuilder.
 * All access is via NodeID or EdgeID. Bulk access is provided for performance-critical
 * workflows, but typical usage is ID-based.
 *
 * @section design Design Principles
 * - Separation of construction (GridBuilder) and runtime usage (Grid)
 * - Immutable topology after construction
 * - ID-based access as primary interface
 * - Optional bulk access for performance
 *
 * @section type_summary Type Dependencies (from Types.h)
 *   - NodeID   -> std::uint32_t : Node identifier
 *   - EdgeID   -> std::uint32_t : Edge identifier
 *   - NodeType -> enum class    : Node classification
 *   - Capacity -> double        : Edge capacity
 *
 * @section invariants Invariants
 * - node_type(id) is defined for all valid NodeID
 * - All edges reference valid node indices
 * - Graph structure is fixed after construction
 *
 * @section performance Performance
 * - Contiguous storage for cache efficiency
 * - Lazy adjacency construction (O(E) first call, O(1) thereafter)
 *
 * @note Intended as read-only after construction.
 */
class Grid {
public:

    // ============================================================
    // Construction
    // ============================================================

    /**
     * @brief Default constructor.
     *
     * Creates an empty grid. Intended for use by GridBuilder.
     */
    Grid() = default;

    // ============================================================
    // Size API
    // ============================================================

    /** @brief Returns number of nodes. */
    [[nodiscard]] size_t num_nodes() const noexcept { return m_nodes.size(); }

    /** @brief Returns number of edges. */
    [[nodiscard]] size_t num_edges() const noexcept { return m_edges.size(); }

    // ============================================================
    // Primary immutable accessors
    // ============================================================

    /** @brief Retrieve node by NodeID. Throws if invalid. */
    [[nodiscard]] const Node& node(NodeID id) const {
        if (id >= m_nodes.size()) throw std::out_of_range("NodeID out of range");
        return m_nodes[id];
    }

    /** @brief Retrieve edge by EdgeID. Throws if invalid. */
    [[nodiscard]] const Edge& edge(EdgeID id) const {
        if (id >= m_edges.size()) throw std::out_of_range("EdgeID out of range");
        return m_edges[id];
    }

    /** @brief Retrieve node type for given NodeID. */
    [[nodiscard]] NodeType node_type(NodeID id) const {
        if (id >= m_node_type.size()) throw std::out_of_range("NodeID out of range");
        return m_node_type[id];
    }

    // ============================================================
    // Bulk accessors (performance)
    // ============================================================

    /** @brief Returns all nodes. Exposes internal storage. */
    [[nodiscard]] const std::vector<Node>& nodes() const noexcept { return m_nodes; }

    /** @brief Returns all edges. Exposes internal storage. */
    [[nodiscard]] const std::vector<Edge>& edges() const noexcept { return m_edges; }

    // ============================================================
    // Derived topology
    // ============================================================

    /**
     * @brief Retrieve adjacency list representation.
     *
     * Lazily constructs adjacency list on first call.
     * Complexity: O(E) first call, O(1) thereafter.
     */
    [[nodiscard]] const std::vector<std::vector<NodeID>>& adjacency() const;

private:

    // ============================================================
    // Builder access
    // ============================================================

    friend class GridBuilder;

    // ============================================================
    // Core topology storage
    // ============================================================

    std::vector<Node> m_nodes;          ///< Node storage
    std::vector<Edge> m_edges;          ///< Edge storage
    std::vector<NodeType> m_node_type;  ///< Node type classification

    // ============================================================
    // Cached derived data
    // ============================================================

    mutable std::vector<std::vector<NodeID>> m_adjacency; ///< Cached adjacency list
    mutable bool m_adjacency_valid = false;               ///< Whether adjacency cache is valid

    // ============================================================
    // Internal helpers
    // ============================================================

    void build_adjacency() const; ///< Build adjacency list from edge list
};