#pragma once

#include <vector>
#include <string>
#include "InjectedGrid.h"
#include "TimeSeries.h"
#include "GridSpec.h"
#include "TimeSeriesAttributes.h"
#include "Types.h"

/**
 * @file PowerGridSnapshot.h
 * @brief Immutable, validated snapshot of a fully assembled power grid dataset.
 *
 * @defgroup power_grid_snapshot PowerGridSnapshot
 * @brief Canonical in-memory representation combining topology, attributes, and time series.
 *
 * PowerGridSnapshot aggregates:
 * - Grid topology + static attributes (InjectedGrid)
 * - Time series data (TimeSeries)
 * - Metadata and specification (GridSpec + TimeSeriesAttributes)
 *
 * It is intended as a read-only, validated structure for:
 * - Export (HDF5, CSV, Parquet, etc.)
 * - Solver or analysis consumption
 *
 * @section design Design Philosophy
 * - Immutable snapshot after construction
 * - Flattened, aligned vectors for high-performance access
 * - Separation of node-level, edge-level, and temporal data
 * - Deterministic validation during construction
 */
class PowerGridSnapshot {
public:

    // ============================================================
    // Construction
    // ============================================================

    /**
     * @brief Construct a fully validated, immutable snapshot.
     *
     * @param grid Immutable injected grid (topology + static attributes)
     * @param ts Time series data aligned to grid nodes
     * @param spec Grid-level metadata/specification
     * @param ts_attr Time series generation metadata
     *
     * @throws std::runtime_error if alignment or consistency checks fail
     */
    PowerGridSnapshot(const InjectedGrid& grid,
                      const TimeSeries& ts,
                      const GridSpec& spec,
                      const TimeSeriesAttributes& ts_attr);

    // ============================================================
    // Node Accessors
    // ============================================================

    [[nodiscard]] size_t num_nodes() const { return m_num_nodes_; }
    [[nodiscard]] const std::vector<NodeID>& node_ids() const { return m_node_ids_; }
    [[nodiscard]] const std::vector<NodeType>& node_types() const { return m_node_types_; }
    [[nodiscard]] const std::vector<Power>& node_base_load() const { return m_node_base_load_; }
    [[nodiscard]] const std::vector<Capacity>& node_max_capacity() const { return m_node_max_capacity_; }

    // ============================================================
    // Edge Accessors
    // ============================================================

    [[nodiscard]] size_t num_edges() const { return m_num_edges_; }
    [[nodiscard]] const std::vector<EdgeID>& edge_ids() const { return m_edge_ids_; }
    [[nodiscard]] const std::vector<NodeID>& edge_from() const { return m_edge_from_; }
    [[nodiscard]] const std::vector<NodeID>& edge_to() const { return m_edge_to_; }
    [[nodiscard]] const std::vector<Power>& edge_reactance() const { return m_edge_reactance_; }
    [[nodiscard]] const std::vector<Capacity>& edge_max_capacity() const { return m_edge_max_capacity_; }

    // ============================================================
    // Time Series Accessors
    // ============================================================

    [[nodiscard]] size_t num_timesteps() const { return m_num_timesteps_; }
    [[nodiscard]] const std::vector<Demand>& ts_demand() const { return m_ts_demand_; }           // [node][t]
    [[nodiscard]] const std::vector<Capacity>& ts_generation() const { return m_ts_generation_; } // [node][t]

    // ============================================================
    // Grid Specification / Metadata
    // ============================================================

    [[nodiscard]] const GridSpec& spec() const { return m_spec_; }
    [[nodiscard]] const TimeSeriesAttributes& ts_attributes() const { return m_ts_attr_; }

private:

    // ============================================================
    // Validation Helper
    // ============================================================

    void validate(const InjectedGrid& grid, const TimeSeries& ts) const;

    // ============================================================
    // Metadata
    // ============================================================

    GridSpec m_spec_;
    TimeSeriesAttributes m_ts_attr_;
    size_t m_num_nodes_;
    size_t m_num_edges_;
    size_t m_num_timesteps_;

    // ============================================================
    // Node-Level Data
    // ============================================================

    std::vector<NodeID> m_node_ids_;
    std::vector<NodeType> m_node_types_;
    std::vector<Power> m_node_base_load_;
    std::vector<Capacity> m_node_max_capacity_;

    // ============================================================
    // Edge-Level Data
    // ============================================================

    std::vector<EdgeID> m_edge_ids_;
    std::vector<NodeID> m_edge_from_;
    std::vector<NodeID> m_edge_to_;
    std::vector<Power> m_edge_reactance_;
    std::vector<Capacity> m_edge_max_capacity_;

    // ============================================================
    // Flattened Time Series Data
    // ============================================================

    std::vector<Demand> m_ts_demand_;        // [node][t]
    std::vector<Capacity> m_ts_generation_;  // [node][t]
};