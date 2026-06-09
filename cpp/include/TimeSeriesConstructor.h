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
 * This class is responsible for constructing a fully deterministic
 * \ref TimeSeries object from an \ref InjectedGrid and a
 * \ref TimeSeriesAttributes configuration.
 *
 * It acts as a stateless construction layer, similar in design to
 * \ref GridInjector:
 *
 * InjectedGrid + TimeSeriesAttributes b
 *
 * @section design Design Philosophy
 * - Stateless construction API
 * - Deterministic outputs given (grid, attributes)
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
 */
class TimeSeriesConstructor {
public:

    /**
     * @brief Construct a full node-wise time series dataset.
     *
     * @param grid Fully injected grid (contains node types + static attributes)
     * @param attr Time series configuration parameters
     *
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
     * Used to simulate diurnal load variation.
     */
    // TimeSeriesConstructor.h
    double demand_modulation(TimeStep t, 
                             TimeStep T, 
                             double amplitude) const;

    /**
     * @brief Generate sinusoidal modulation factor for generation.
     *
     * Can be disabled via amplitude = 0.
     */
    double generation_modulation(TimeStep t,
                                 TimeStep T, 
                                 double amplitude) const;        

    /**
     * @brief Deterministic RNG seed initialization helper.
     */
    std::uint64_t make_seed(std::uint64_t base_seed,
                            NodeID node_id,
                            bool is_generator) const;
};
