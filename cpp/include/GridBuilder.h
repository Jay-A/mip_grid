#pragma once

#include <cstdint>
#include <stdexcept>

#include "Grid.h"
#include "Node.h"
#include "Edge.h"
#include "Types.h"

/**
 * @file GridBuilder.h
 * @brief Mutable construction interface for immutable Grid objects.
 *
 * @defgroup grid_builder GridBuilder
 * @brief Controlled construction layer for Grid generation.
 *
 * GridBuilder provides a controlled, incremental API for constructing a Grid.
 * It is the only component allowed to mutate graph structure before finalization.
 * After build(), ownership is transferred into an immutable Grid instance.
 *
 * @section design Design Philosophy
 * - Separate mutation (build phase) from runtime usage
 * - Ensure deterministic and validated graph construction
 * - Use move semantics for zero-copy transfer
 * - Enforce ID-based topology semantics (no raw container exposure)
 *
 * @section type_summary Type Dependencies (from Types.h)
 * - NodeID   -> std::uint32_t : Node identifier
 * - EdgeID   -> std::uint32_t : Edge identifier
 * - NodeType -> enum class    : Node classification
 *
 * @note GridBuilder is not thread-safe.
 */

/**
 * @class GridBuilder
 * @ingroup grid_builder
 * @brief Builder for constructing immutable Grid instances.
 *
 * GridBuilder incrementally constructs:
 * - Nodes
 * - Directed edges
 * - Node type metadata
 *
 * The resulting graph is transferred into an immutable Grid via move semantics.
 *
 * @section lifecycle Lifecycle
 * 1. Construct GridBuilder
 * 2. Add nodes and edges
 * 3. Assign node types
 * 4. Call build() to finalize
 *
 * After build(), the builder must not be used.
 */
class GridBuilder {
public:

    // ============================================================
    // Construction
    // ============================================================

    /**
     * @brief Construct a graph builder with optional pre-allocation.
     *
     * @param n Initial number of nodes to reserve or create.
     *
     * @note Useful for large graphs (10k+ nodes) to reduce reallocations.
     */
    explicit GridBuilder(NodeID n = 0);

    // ============================================================
    // Safety: disable copying
    // ============================================================

    GridBuilder(const GridBuilder&) = delete;
    GridBuilder& operator=(const GridBuilder&) = delete;

    GridBuilder(GridBuilder&&) = default;
    GridBuilder& operator=(GridBuilder&&) = default;

    // ============================================================
    // Graph construction API
    // ============================================================

    /**
     * @brief Add a new node to the graph.
     *
     * @return NodeID Identifier of the newly created node (uint32_t)
     *
     * @details Node IDs are assigned sequentially starting from 0.
     *
     * @see Grid, NodeType, add_edge()
     */
    NodeID add_node();

    /**
     * @brief Assign a functional type to a node.
     *
     * @param id Node identifier (NodeID / uint32_t)
     * @param type Node role classification (NodeType)
     *
     * @throws std::out_of_range if NodeID is invalid.
     */
    void set_node_type(NodeID id, NodeType type);

    /**
     * @brief Add a directed edge between two nodes.
     *
     * @param from Source node identifier (NodeID / uint32_t)
     * @param to Destination node identifier (NodeID / uint32_t)
     *
     * @throws std::out_of_range if either NodeID is invalid.
     *
     * @note This graph is directed; bidirectional connectivity must be
     *       explicitly added via two calls.
     */
    void add_edge(NodeID from, NodeID to);

    // ============================================================
    // Read-only inspection API (builder-safe)
    // ============================================================

    /**
     * @brief Get number of nodes currently in the builder.
     *
     * @return NodeID / uint32_t
     */
    [[nodiscard]] NodeID num_nodes() const noexcept;

    /**
     * @brief Get number of edges currently in the builder.
     *
     * @return EdgeID / uint32_t
     */
    [[nodiscard]] NodeID num_edges() const noexcept;

    /**
     * @brief Inspect node type (builder-time view).
     *
     * @param id NodeID / uint32_t
     *
     * @return NodeType
     *
     * @throws std::out_of_range if NodeID is invalid.
     */
    [[nodiscard]] NodeType node_type(NodeID id) const;

    /**
     * @brief Read-only access to edges during construction.
     *
     * @warning Intended only for topology analysis during build phase.
     * Must not be used after build().
     *
     * @return const reference to vector of Edge
     */
    [[nodiscard]] const std::vector<Edge>& edges() const noexcept;

    /**
     * @brief Read-only access to node type vector during construction.
     *
     * @return const reference to vector of NodeType
     */
    [[nodiscard]] const std::vector<NodeType>& node_types() const noexcept;

    /**
     * @brief Inspect node existence via ID.
     *
     * @param id NodeID / uint32_t
     * @return true if node exists, false otherwise
     */
    [[nodiscard]] bool has_node(NodeID id) const noexcept;

    // ============================================================
    // Finalization
    // ============================================================

    /**
     * @brief Finalize construction and transfer ownership to Grid.
     *
     * @details
     * This function is rvalue-qualified to enforce single-use semantics:
     *
     * @code
     * Grid grid = std::move(builder).build();
     * @endcode
     *
     * @return Immutable Grid containing fully constructed topology.
     *
     * @post Builder must not be used after this call.
     */
    Grid build() &&;

    /**
     * @brief Deleted lvalue build to enforce move-only finalization.
     */
    Grid build() & = delete;

private:

    // ============================================================
    // Internal construction state
    // ============================================================

    /// Internal Grid instance under construction
    Grid m_grid;

    /**
     * @brief Internal validation helper.
     *
     * Ensures graph consistency before finalization.
     *
     * @throws std::runtime_error if the graph violates invariants.
     */
    void validate() const;
};