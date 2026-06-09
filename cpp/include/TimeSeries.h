#pragma once

#include <cstdint>
#include <vector>
#include <stdexcept>

#include "InjectedGrid.h"
#include "Types.h"

/**
 * @file TimeSeries.h
 * @brief Immutable container for node-wise time series data.
 *
 * This class stores deterministic time-dependent profiles for all nodes
 * in a synthetic power grid. It is produced exclusively by
 * \ref TimeSeriesConstructor and is immutable after construction.
 *
 * @section design Design Philosophy
 * - Fully immutable after construction
 * - Node-aligned indexing with InjectedGrid
 * - Cache-friendly contiguous storage
 * - No dependency on Grid topology (only InjectedGrid metadata if needed)
 *
 * @section data_layout Data Layout
 * Data is stored in node-major format:
 *
 * - demand[node_id][t]
 * - generation[node_id][t]
 *
 * This enables efficient solver-side slicing per node.
 *
 * @section usage Example
 * @code
 * const auto& series = ts.demand(node_id);
 * double value = series[t];
 * @endcode
 */
class TimeSeries {
public:

    /**
     * @brief Construct full immutable time series object.
     *
     * @param grid Reference to injected grid (for node alignment validation)
     * @param demand_time_series Node-wise demand [N x T]
     * @param generation_time_series Node-wise generation [N x T]
     *
     * @throws std::runtime_error if dimensions do not match grid
     */
    TimeSeries(const InjectedGrid& grid,
               std::vector<std::vector<double>> demand_time_series,
               std::vector<std::vector<double>> generation_time_series);

    /**
     * @brief Number of nodes in the dataset.
     */
    std::size_t num_nodes() const noexcept;

    /**
     * @brief Number of timesteps per node.
     */
    std::size_t num_timesteps() const noexcept;

    /**
     * @brief Access full demand series for a node.
     *
     * @param node_id Node index
     * @return const reference to time series vector
     */
    const std::vector<double>& demand(NodeID node_id) const;

    /**
     * @brief Access full generation series for a node.
     *
     * @param node_id Node index
     * @return const reference to time series vector
     */
    const std::vector<double>& generation(NodeID node_id) const;

    /**
     * @brief Access single demand value.
     *
     * @param node_id Node index
     * @param t timestep index
     */
    double demand(NodeID node_id, TimeStep t) const;

    /**
     * @brief Access single generation value.
     *
     * @param node_id Node index
     * @param t timestep index
     */
    double generation(NodeID node_id, std::size_t t) const;

    /**
     * @brief Get raw demand matrix (read-only).
     */
    const std::vector<std::vector<double>>& demand_matrix() const noexcept;

    /**
     * @brief Get raw generation matrix (read-only).
     */
    const std::vector<std::vector<double>>& generation_matrix() const noexcept;

private:

    // ============================================================
    // Core immutable storage
    // ============================================================

    /// Node-wise demand: [node][time]
    const std::vector<std::vector<double>> m_demand;

    /// Node-wise generation: [node][time]
    const std::vector<std::vector<double>> m_generation;

    /// Cached dimensions
    const std::size_t m_num_nodes;
    const std::size_t m_num_timesteps;
};
