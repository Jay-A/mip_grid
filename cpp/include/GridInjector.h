#pragma once

#include <cstdint>
#include <random>
#include <vector>

#include "Types.h"             // for NodeID, EdgeID, NodeType, etc.
#include "GridAttributes.h"    // for NodeAttributes, EdgeAttributes
#include "Grid.h"
#include "GridSpec.h"
#include "InjectedGrid.h"

/**
 * @file GridInjector.h
 * @brief Physical parameter injection layer for synthetic grid generation.
 *
 * This class is responsible for transforming a purely topological Grid into
 * a fully parameterized InjectedGrid containing:
 * - generator capacities
 * - load demands
 * - edge capacities and impedances
 * - operational flags
 *
 * The injector is deterministic given (Grid, GridSpec, seed).
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
 * @section usage Example
 * @code
 * GridInjector injector(42);
 * InjectedGrid ig = injector.inject(grid, spec);
 * @endcode
 */
class GridInjector {
public:

    /**
     * @brief Construct injector with deterministic seed.
     *
     * @param seed Global RNG seed for reproducibility.
     */
    explicit GridInjector(uint64_t seed = 0);

    /**
     * @brief Inject physical parameters into a topological grid.
     *
     * This function produces a fully frozen InjectedGrid snapshot.
     *
     * @param grid Input topological grid (immutable)
     * @param spec Grid generation specification (controls distributions)
     *
     * @return Fully parameterized InjectedGrid (immutable snapshot)
     *
     * @note This function is deterministic for identical (grid, spec, seed)
     */
    InjectedGrid inject(const Grid& grid, const GridSpec& spec);

private:

    // ------------------------------------------------------------
    // RNG state (encapsulated, deterministic)
    // ------------------------------------------------------------
    std::mt19937_64 m_rng;

    // ------------------------------------------------------------
    // Internal helper distributions
    // ------------------------------------------------------------
    double uniform01();

    double sample_generator_capacity(const GridSpec& spec);
    double sample_load_demand(const GridSpec& spec);
    double sample_line_capacity(const GridSpec& spec);

    double sample_reactance(const GridSpec& spec);

    bool sample_operational(double failure_rate);

    // ------------------------------------------------------------
    // Topology-aware helpers
    // ------------------------------------------------------------
    double node_degree_weight(NodeID id, const Grid& grid) const;

    bool is_cross_link(NodeID from, NodeID to, const Grid& grid) const;

    // ------------------------------------------------------------
    // Injection builders
    // ------------------------------------------------------------
    std::vector<NodeAttributes>
    build_node_attributes(const Grid& grid, const GridSpec& spec);

    std::vector<EdgeAttributes>
    build_edge_attributes(const Grid& grid, const GridSpec& spec);
};
