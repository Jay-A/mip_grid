#pragma once

#include <cstdint>
#include <random>
#include <vector>

#include "Types.h"             // NodeID, EdgeID, NodeType, etc.
#include "GridAttributes.h"    // NodeAttributes, EdgeAttributes
#include "Grid.h"
#include "GridSpec.h"
#include "InjectedGrid.h"

/**
 * @file GridInjector.h
 * @brief Physical parameter injection layer for synthetic grids.
 *
 * @defgroup grid_injector GridInjector
 * @brief Deterministic mapping from topological Grid to parameterized InjectedGrid.
 *
 * GridInjector transforms a purely topological Grid into a fully parameterized
 * InjectedGrid, assigning:
 * - generator capacities
 * - load demands
 * - edge capacities and impedances
 * - operational flags
 *
 * @section design Design Philosophy
 * - Separation of topology and physics
 * - Fully deterministic construction (MPI-safe)
 * - No mutation of Grid or InjectedGrid after construction
 * - Single responsibility: parameterization only
 *
 * @section mpi MPI Considerations
 * Each MPI rank may construct an independent injector instance.
 * Determinism is guaranteed via seeded RNG (optionally rank-offset).
 *
 * @section type_summary Type dependencies
 * - NodeID           -> std::uint32_t : Node identifier
 * - EdgeID           -> std::uint32_t : Edge identifier
 * - NodeType         -> enum class    : Node classification
 * - NodeAttributes   -> struct        : Node physical & operational properties
 * - EdgeAttributes   -> struct        : Edge physical & operational properties
 * - Grid             -> class         : Topological grid container
 * - GridSpec         -> class         : Declarative grid specification
 * - InjectedGrid     -> class         : Parameterized grid snapshot
 *
 * @section usage Example
 * @code
 * GridInjector injector(42);
 * InjectedGrid ig = injector.inject(grid, spec);
 * @endcode
 */
class GridInjector {
public:

    // ============================================================
    // Construction
    // ============================================================

    /**
     * @brief Construct injector with optional deterministic seed.
     *
     * @param seed Global RNG seed for reproducibility (uint64_t)
     */
    explicit GridInjector(uint64_t seed = 0);

    // ============================================================
    // Injection API
    // ============================================================

    /**
     * @brief Inject physical parameters into a topological grid.
     *
     * @param grid Input topological grid (immutable)
     * @param spec GridSpec controlling distributions for injection
     * @return InjectedGrid Fully parameterized immutable grid snapshot
     *
     * @note Deterministic for identical (grid, spec, seed)
     */
    InjectedGrid inject(const Grid& grid, const GridSpec& spec);

private:

    // ============================================================
    // Internal RNG state
    // ============================================================
    std::mt19937_64 m_rng; ///< Encapsulated deterministic RNG

    // ============================================================
    // Helper distributions
    // ============================================================
    double uniform01();

    double sample_generator_capacity(const GridSpec& spec);
    double sample_load_demand(const GridSpec& spec);
    double sample_line_capacity(const GridSpec& spec);
    double sample_reactance(const GridSpec& spec);
    bool sample_operational(double failure_rate);

    // ============================================================
    // Topology-aware helpers
    // ============================================================
    double node_degree_weight(NodeID id, const Grid& grid) const;
    bool is_cross_link(NodeID from, NodeID to, const Grid& grid) const;

    // ============================================================
    // Injection builders
    // ============================================================
    std::vector<NodeAttributes> build_node_attributes(const Grid& grid, const GridSpec& spec);
    std::vector<EdgeAttributes> build_edge_attributes(const Grid& grid, const GridSpec& spec);
};