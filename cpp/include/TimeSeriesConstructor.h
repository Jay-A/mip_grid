#pragma once

#include <cstdint>
#include <vector>

#include "TimeSeriesAttributes.h"
#include "InjectedGrid.h"
#include "TimeSeries.h"

/**
 * @file TimeSeriesConstructor.h
 * @brief Deterministic generator for node-wise time series data.
 *
 * @defgroup ts_constructor TimeSeriesConstructor
 * @brief Stateless builder for fully deterministic TimeSeries objects.
 *
 * TimeSeriesConstructor produces immutable \ref TimeSeries objects from a
 * given \ref InjectedGrid and \ref TimeSeriesAttributes configuration.
 * It enforces:
 * - Deterministic output for identical inputs
 * - No mutation of grid or attributes
 * - Separation of grid physics from dynamic time series construction
 *
 * @section design Design Philosophy
 * - Stateless construction API
 * - Deterministic outputs given (grid + attributes)
 * - No mutation of inputs
 * - Separation of physics (InjectedGrid) and dynamics (TimeSeries)
 *
 * @section scope Scope of Responsibility
 * This class is responsible only for:
 * - Node-wise demand generation (Load nodes)
 * - Node-wise generation profiles (Generator nodes)
 * - Zero-series handling for Bus nodes
 * - Deterministic modulation (e.g., sinusoidal daily cycles)
 *
 * It does NOT:
 * - Modify grid topology
 * - Perform power flow or optimization
 * - Introduce external dependencies (MPI, solvers, etc.)
 *
 * @section usage Example
 * @code
 * TimeSeriesConstructor constructor;
 * TimeSeries ts = constructor.construct(injected_grid, attributes);
 * @endcode
 *
 * @section type_summary Type dependencies
 * - NodeID   -> Node index (uint32_t)
 * - TimeStep -> Timestep index (uint32_t / size_t)
 * - double   -> Modulation factors
 * - uint64_t -> RNG seed
 */
class TimeSeriesConstructor {
public:

    /**
     * @brief Construct a full node-wise time series dataset.
     *
     * @param grid Fully injected grid (contains node types + static attributes)
     * @param attr Time series configuration parameters
     * @return Immutable TimeSeries object containing all node-wise trajectories
     *
     * @note Deterministic for identical inputs (grid + attr)
     */
    TimeSeries construct(const InjectedGrid& grid,
                         const TimeSeriesAttributes& attr) const;

private:

    // ============================================================
    // Internal deterministic helpers
    // ============================================================

    /**
     * @brief Generate sinusoidal modulation factor for demand.
     *
     * Simulates diurnal load variation.
     *
     * @param t Current timestep
     * @param T Total number of timesteps
     * @param amplitude Modulation amplitude [0.0, 1.0]
     * @return Multiplicative modulation factor
     */
    double demand_modulation(TimeStep t, 
                             TimeStep T, 
                             double amplitude) const;

    /**
     * @brief Generate sinusoidal modulation factor for generation.
     *
     * Can be disabled by setting amplitude = 0.
     *
     * @param t Current timestep
     * @param T Total number of timesteps
     * @param amplitude Modulation amplitude [0.0, 1.0]
     * @return Multiplicative modulation factor
     */
    double generation_modulation(TimeStep t,
                                 TimeStep T, 
                                 double amplitude) const;        

    /**
     * @brief Deterministic RNG seed initialization helper.
     *
     * Combines base seed with node ID and generator flag to produce unique seeds.
     *
     * @param base_seed Base RNG seed
     * @param node_id Node index
     * @param is_generator True if node is a generator, false otherwise
     * @return Deterministic per-node RNG seed
     */
    std::uint64_t make_seed(std::uint64_t base_seed,
                            NodeID node_id,
                            bool is_generator) const;
};