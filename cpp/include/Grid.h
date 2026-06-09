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
 * @brief Graph container for synthetic power grid topology.
 *
 * @defgroup grid_module Grid
 * @brief Core topology representation for synthetic power grids.
 *
 * This module defines the immutable graph structure used throughout
 * the mip_grid pipeline. It is constructed by GridBuilder and consumed
 * by downstream simulation and injection layers.
 *
 * @section design Design Philosophy
 * - Separation of construction and runtime usage
 * - Immutable topology after construction
 * - ID-based access as the primary interface
 * - Optional bulk access for performance-critical workflows
 *
 * @note This class is intended to be used as a read-only data structure
 * after construction.
 */

/**
 * @class Grid
 * @ingroup grid_module
 * @brief Directed graph container representing grid topology.
 *
 * The Grid class stores nodes, edges, and node-type metadata.
 * It provides a minimal, immutable interface for traversal and inspection.
 *
 * @section invariants Invariants
 * - node_type(id) is defined for all valid NodeID
 * - All edges reference valid node indices
 * - Graph structure is fixed after construction
 *
 * @section performance Performance Characteristics
 * - Contiguous storage for cache efficiency
 * - Lazy adjacency construction (O(E) first call, O(1) thereafter)
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

    /**
     * @brief Get number of nodes in the grid.
     *
     * @return Number of nodes.
     */
    [[nodiscard]] size_t num_nodes() const noexcept {
        return m_nodes.size();
    }

    /**
     * @brief Get number of edges in the grid.
     *
     * @return Number of edges.
     */
    [[nodiscard]] size_t num_edges() const noexcept {
        return m_edges.size();
    }

    // ============================================================
    // Primary immutable accessors (ID-based API)
    // ============================================================

    /**
     * @brief Retrieve a node by NodeID.
     *
     * @throws std::out_of_range if id is invalid
     */
    [[nodiscard]] const Node& node(NodeID id) const {
        if (id >= m_nodes.size()) {
            throw std::out_of_range("NodeID out of range");
        }
        return m_nodes[id];
    }

    /**
     * @brief Retrieve an edge by EdgeID.
     *
     * @throws std::out_of_range if id is invalid
     */
    [[nodiscard]] const Edge& edge(EdgeID id) const {
        if (id >= m_edges.size()) {
            throw std::out_of_range("EdgeID out of range");
        }
        return m_edges[id];
    }

    /**
     * @brief Retrieve node type classification for a given node.
     *
     * @throws std::out_of_range if id is invalid
     */
    [[nodiscard]] NodeType node_type(NodeID id) const {
        if (id >= m_node_type.size()) {
            throw std::out_of_range("NodeID out of range");
        }
        return m_node_type[id];
    }

    // ============================================================
    // Bulk accessors (performance / preprocessing only)
    // ============================================================

    /**
     * @brief Retrieve all nodes.
     *
     * @warning Exposes internal storage layout. Prefer node(id) for general use.
     */
    [[nodiscard]] const std::vector<Node>& nodes() const noexcept {
        return m_nodes;
    }

    /**
     * @brief Retrieve all edges.
     *
     * @warning Exposes internal storage layout. Prefer edge(id) for general use.
     */
    [[nodiscard]] const std::vector<Edge>& edges() const noexcept {
        return m_edges;
    }

    // ============================================================
    // Derived topology
    // ============================================================

    /**
     * @brief Retrieve adjacency list representation.
     *
     * @details
     * Lazily constructs adjacency list on first call.
     *
     * @return Const reference to adjacency list.
     *
     * @note Complexity: O(E) first call, O(1) thereafter.
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

    /// Node storage
    std::vector<Node> m_nodes;

    /// Edge storage
    std::vector<Edge> m_edges;

    /// Node type classification (Generator / Load / Bus)
    std::vector<NodeType> m_node_type;

    // ============================================================
    // Cached derived data
    // ============================================================

    /// Cached adjacency list (lazy computed)
    mutable std::vector<std::vector<NodeID>> m_adjacency;

    /// Whether adjacency cache is valid
    mutable bool m_adjacency_valid = false;

    // ============================================================
    // Internal helpers
    // ============================================================

    /// Build adjacency list from edge list
    void build_adjacency() const;
};