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
 * @defgroup time_series TimeSeries
 * @brief Stores deterministic demand and generation profiles for all nodes.
 *
 * TimeSeries holds fully immutable, node-aligned time-dependent data for
 * synthetic power grids. Produced exclusively by \ref TimeSeriesConstructor,
 * it ensures:
 * - Immutability after construction
 * - Node-major contiguous storage for cache efficiency
 * - Alignment with InjectedGrid metadata without depending on topology
 *
 * @section design Design Philosophy
 * - Fully immutable after construction
 * - Node-aligned indexing with InjectedGrid
 * - Cache-friendly contiguous storage
 * - No dependency on grid topology (only InjectedGrid metadata)
 *
 * @section data_layout Data Layout
 * Data is stored as node-major matrices:
 *
 * - demand[node_id][t]
 * - generation[node_id][t]
 *
 * This enables efficient solver-side per-node slicing.
 *
 * @section usage Example
 * @code
 * const auto& series = ts.demand(node_id);
 * double value = series[t];
 * @endcode
 *
 * @section type_summary Type dependencies
 * - NodeID   -> Node index (uint32_t)
 * - TimeStep -> Timestep index (uint32_t / size_t)
 * - double   -> Continuous time series values
 */
class TimeSeries {
public:

    /**
     * @brief Construct full immutable time series object.
     *
     * @param grid Reference to injected grid for node alignment validation
     * @param demand_time_series Node-wise demand [N x T]
     * @param generation_time_series Node-wise generation [N x T]
     *
     * @throws std::runtime_error if dimensions do not match grid
     */
    TimeSeries(const InjectedGrid& grid,
               std::vector<std::vector<double>> demand_time_series,
               std::vector<std::vector<double>> generation_time_series);

    /**
     * @brief Get number of nodes in the dataset.
     * @return Node count
     */
    std::size_t num_nodes() const noexcept;

    /**
     * @brief Get number of timesteps per node.
     * @return Timesteps per node
     */
    std::size_t num_timesteps() const noexcept;

    /**
     * @brief Access full demand series for a node.
     *
     * @param node_id Node index
     * @return Const reference to node demand vector
     */
    const std::vector<double>& demand(NodeID node_id) const;

    /**
     * @brief Access full generation series for a node.
     *
     * @param node_id Node index
     * @return Const reference to node generation vector
     */
    const std::vector<double>& generation(NodeID node_id) const;

    /**
     * @brief Access single demand value.
     *
     * @param node_id Node index
     * @param t Timestep index
     * @return Demand at given node and timestep
     */
    double demand(NodeID node_id, TimeStep t) const;

    /**
     * @brief Access single generation value.
     *
     * @param node_id Node index
     * @param t Timestep index
     * @return Generation at given node and timestep
     */
    double generation(NodeID node_id, std::size_t t) const;

    /**
     * @brief Get full demand matrix (read-only).
     * @return Node-major demand matrix [N x T]
     */
    const std::vector<std::vector<double>>& demand_matrix() const noexcept;

    /**
     * @brief Get full generation matrix (read-only).
     * @return Node-major generation matrix [N x T]
     */
    const std::vector<std::vector<double>>& generation_matrix() const noexcept;

private:

    // ============================================================
    // Core immutable storage
    // ============================================================

    /// Node-wise demand matrix [node][time]
    const std::vector<std::vector<double>> m_demand;

    /// Node-wise generation matrix [node][time]
    const std::vector<std::vector<double>> m_generation;

    /// Cached number of nodes
    const std::size_t m_num_nodes;

    /// Cached number of timesteps per node
    const std::size_t m_num_timesteps;
};