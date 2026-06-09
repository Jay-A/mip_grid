#pragma once

#include "GridAttributes.h"
#include "Types.h"
#include "Grid.h"

#include <vector>
#include <stdexcept>

/**
 * @file InjectedGrid.h
 * @brief Immutable container binding Grid topology with physical attributes.
 *
 * @defgroup injected_grid InjectedGrid
 * @brief Combined representation of grid topology and injected physical parameters.
 *
 * InjectedGrid is a post-processing artifact produced exclusively by GridInjector.
 * It couples:
 * - Grid topology (nodes/edges)
 * - NodeAttributes (generation/load/operational state)
 * - EdgeAttributes (capacity/reactance/operational state)
 *
 * It is a read-only snapshot intended for downstream simulation and optimization.
 *
 * @section design Design Philosophy
 * - Separation of topology (Grid) and physics (attributes)
 * - Immutable snapshot after construction
 * - Ownership transfer of attribute containers
 * - Structural validation at construction time only
 *
 * @section type_summary Type Dependencies
 * - NodeID         -> std::uint32_t : Node identifier
 * - EdgeID         -> std::uint32_t : Edge identifier
 * - Grid           -> class         : Graph topology
 * - NodeAttributes -> struct        : Node physical state
 * - EdgeAttributes -> struct        : Edge physical state
 */

/**
 * @class InjectedGrid
 * @ingroup injected_grid
 * @brief Combined grid snapshot with topology and physical attributes.
 *
 * InjectedGrid is produced by GridInjector and provides a unified,
 * read-only representation of a fully parameterized power grid.
 */
class InjectedGrid {
public:

    // ============================================================
    // Construction
    // ============================================================

    /**
     * @brief Construct a fully initialized InjectedGrid.
     *
     * @param g Input topological grid
     * @param node_attrs Node-level physical attributes (moved in)
     * @param edge_attrs Edge-level physical attributes (moved in)
     *
     * @throws std::runtime_error if attribute sizes do not match grid topology
     */
    InjectedGrid(const Grid& g,
                 std::vector<NodeAttributes>&& node_attrs,
                 std::vector<EdgeAttributes>&& edge_attrs)
        : grid_(g),
          nodes_(std::move(node_attrs)),
          edges_(std::move(edge_attrs))
    {
        if (nodes_.size() != grid_.num_nodes()) {
            throw std::runtime_error("Injected node attributes size mismatch");
        }

        if (edges_.size() != grid_.num_edges()) {
            throw std::runtime_error("Injected edge attributes size mismatch");
        }
    }

    // ============================================================
    // Deleted default construction
    // ============================================================

    InjectedGrid() = delete;

    // ============================================================
    // Copy / move semantics
    // ============================================================

    InjectedGrid(const InjectedGrid&) = default;
    InjectedGrid(InjectedGrid&&) = default;
    InjectedGrid& operator=(const InjectedGrid&) = default;
    InjectedGrid& operator=(InjectedGrid&&) = default;

    // ============================================================
    // Accessors
    // ============================================================

    /**
     * @brief Access underlying grid topology.
     */
    const Grid& grid() const { return grid_; }

    /**
     * @brief Access node attributes.
     */
    const std::vector<NodeAttributes>& nodes() const { return nodes_; }

    /**
     * @brief Access edge attributes.
     */
    const std::vector<EdgeAttributes>& edges() const { return edges_; }

    /**
     * @brief Retrieve node attributes by NodeID.
     *
     * @throws std::out_of_range if id is invalid
     */
    const NodeAttributes& node(NodeID id) const { return nodes_.at(id); }

    /**
     * @brief Retrieve edge attributes by EdgeID.
     *
     * @throws std::out_of_range if id is invalid
     */
    const EdgeAttributes& edge(EdgeID id) const { return edges_.at(id); }

private:

    // ============================================================
    // Internal state
    // ============================================================

    const Grid& grid_;                      ///< Topology reference
    std::vector<NodeAttributes> nodes_;     ///< Node physical attributes
    std::vector<EdgeAttributes> edges_;     ///< Edge physical attributes
};