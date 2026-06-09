#pragma once

#include <cstddef>
#include <cstdint>

/**
 * @file GridSpec.h
 * @brief Declarative configuration for deterministic synthetic grid generation.
 *
 * @defgroup grid_spec GridSpec
 * @brief Parameter container for synthetic grid construction.
 *
 * GridSpec is the single source of truth for building synthetic grid topologies.
 * It does NOT contain runtime graph state. It only defines:
 * - Node counts and functional types
 * - Graph topology constraints
 * - Stochastic physical parameters (for injection)
 * - Deterministic seed for reproducibility
 *
 * @section type_summary Type dependencies
 * - size_t  -> Node/edge counts
 * - double  -> Continuous parameters (ratios, capacities, probabilities)
 * - uint64_t -> RNG seed
 *
 * @section usage Example
 * @code
 * GridSpec spec = GridSpec::Minimal(100);
 * spec.generator_ratio = 0.1;
 * spec.cross_link_rate = 0.05;
 * @endcode
 */
struct GridSpec {

    // ============================================================
    // Topology parameters
    // ============================================================

    size_t num_nodes = 0;                     ///< Total number of nodes in the grid
    double generator_ratio = 0.0;             ///< Fraction of nodes that are generators
    double load_ratio = 0.0;                  ///< Fraction of nodes that are loads
    bool require_connected_graph = true;      ///< Require single connected component
    bool require_generator_reachability = true; ///< Loads must be reachable from at least one generator
    double max_unserved_load_fraction = 0.0;  ///< Max fraction of loads that can be unserved

    size_t branch_factor = 2;                 ///< Branching factor for hierarchical expansion
    bool use_ring_backbone = false;           ///< Backbone topology: ring or tree
    bool add_cross_links = false;             ///< Add sparse long-range cross-links
    double cross_link_rate = 0.0;             ///< Probability of adding a cross-link

    uint64_t seed = 0;                        ///< Deterministic seed for reproducibility

    // ============================================================
    // Stochastic physics parameters (for injector)
    // ============================================================

    // Node-level generator capacities (MW)
    double generator_capacity_min = 50.0;     ///< Minimum generator capacity
    double generator_capacity_max = 500.0;    ///< Maximum generator capacity

    // Node-level acceptable load demand range (MW)
    double load_demand_min = 5.0;             ///< Minimum load demand
    double load_demand_max = 100.0;           ///< Maximum load demand

    // Edge-level capacity ranges (MW)
    double edge_capacity_min = 0.0;
    double edge_capacity_max = 0.0;

    // Line reactance ranges (per-unit)
    double edge_reactance_min = 0.0;
    double edge_reactance_max = 0.0;

    // Node/edge failure probabilities
    double node_failure_prob = 0.0;
    double edge_failure_prob = 0.0;

    // ============================================================
    // Convenience factory
    // ============================================================

    /**
     * @brief Construct a minimal GridSpec for testing or small networks.
     *
     * @param n Number of nodes
     * @return GridSpec initialized with minimal valid defaults
     */
    static GridSpec Minimal(size_t n)
    {
        GridSpec spec;
        spec.num_nodes = n;

        // Minimal generator/load ratios
        spec.generator_ratio = 1.0 / static_cast<double>(n);
        double bus_ratio = n > 5 ? 3.0 / static_cast<double>(n) : 0.0;
        spec.load_ratio = 1.0 - spec.generator_ratio - bus_ratio;

        // Topology defaults
        spec.branch_factor = 2;
        spec.use_ring_backbone = false;
        spec.add_cross_links = false;
        spec.cross_link_rate = 0.0;
        spec.require_connected_graph = true;
        spec.require_generator_reachability = true;
        spec.max_unserved_load_fraction = 0.0;

        // Physics defaults
        spec.generator_capacity_min = 50.0;
        spec.generator_capacity_max = 500.0;
        spec.load_demand_min = 5.0;
        spec.load_demand_max = 100.0;

        spec.edge_capacity_min = 50.0;
        spec.edge_capacity_max = 500.0;
        spec.edge_reactance_min = 0.01;
        spec.edge_reactance_max = 0.1;
        spec.node_failure_prob = 0.0;
        spec.edge_failure_prob = 0.0;

        spec.seed = 1;

        return spec;
    }
};