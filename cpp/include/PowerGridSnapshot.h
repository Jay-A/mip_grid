#pragma once

#include <vector>
#include <string>
#include "InjectedGrid.h"
#include "TimeSeries.h"
#include "GridSpec.h"
#include "TimeSeriesAttributes.h"  // <-- new include
#include "Types.h"  // NodeID, EdgeID, TimeStep, Power, Demand, Capacity, NodeType

/**
 * @file PowerGridSnapshot.h
 * @brief Immutable, validated snapshot of a fully assembled power grid dataset.
 *
 * Combines topology (InjectedGrid), time series (TimeSeries),
 * and metadata (GridSpec + TimeSeriesAttributes) into a single canonical, export-ready structure.
 *
 * This is the final in-memory representation before serialization (e.g. HDF5)
 * or consumption by optimization solvers.
 */
class PowerGridSnapshot {
public:
    /**
     * @brief Construct a fully validated, immutable snapshot.
     *
     * @param grid Immutable injected grid (topology + static attributes)
     * @param ts Time series data aligned to grid nodes
     * @param spec Grid-level metadata/specification
     * @param ts_attr Time series generation attributes (metadata)
     *
     * @throws std::runtime_error if alignment or consistency checks fail
     */
    PowerGridSnapshot(const InjectedGrid& grid,
                      const TimeSeries& ts,
                      const GridSpec& spec,
                      const TimeSeriesAttributes& ts_attr);

    // =========================
    // Node Accessors
    // =========================
    size_t num_nodes() const { return m_num_nodes_; }
    const std::vector<NodeID>& node_ids() const { return m_node_ids_; }
    const std::vector<NodeType>& node_types() const { return m_node_types_; }
    const std::vector<Power>& node_base_load() const { return m_node_base_load_; }
    const std::vector<Capacity>& node_max_capacity() const { return m_node_max_capacity_; }

    // =========================
    // Edge Accessors
    // =========================
    size_t num_edges() const { return m_num_edges_; }
    const std::vector<EdgeID>& edge_ids() const { return m_edge_ids_; }
    const std::vector<NodeID>& edge_from() const { return m_edge_from_; }
    const std::vector<NodeID>& edge_to() const { return m_edge_to_; }
    const std::vector<Power>& edge_reactance() const { return m_edge_reactance_; }
    const std::vector<Capacity>& edge_max_capacity() const { return m_edge_max_capacity_; }

    // =========================
    // Time Series Accessors
    // =========================
    size_t num_timesteps() const { return m_num_timesteps_; }
    const std::vector<Demand>& ts_demand() const { return m_ts_demand_; }         // flattened [node][t]
    const std::vector<Capacity>& ts_generation() const { return m_ts_generation_; } // flattened [node][t]

    // =========================
    // Grid Specification
    // =========================
    const GridSpec& spec() const { return m_spec_; }

    // =========================
    // Time Series Attributes
    // =========================
    const TimeSeriesAttributes& ts_attributes() const { return m_ts_attr_; }

private:
    // Validation helper
    void validate(const InjectedGrid& grid, const TimeSeries& ts) const;

    // Metadata
    GridSpec m_spec_;
    TimeSeriesAttributes m_ts_attr_;   // <-- new member
    size_t m_num_nodes_;
    size_t m_num_edges_;
    size_t m_num_timesteps_;

    // Node-level data
    std::vector<NodeID> m_node_ids_;
    std::vector<NodeType> m_node_types_;
    std::vector<Power> m_node_base_load_;
    std::vector<Capacity> m_node_max_capacity_;

    // Edge-level data
    std::vector<EdgeID> m_edge_ids_;
    std::vector<NodeID> m_edge_from_;
    std::vector<NodeID> m_edge_to_;
    std::vector<Power> m_edge_reactance_;
    std::vector<Capacity> m_edge_max_capacity_;

    // Flattened time series
    std::vector<Demand> m_ts_demand_;         // [node][t]
    std::vector<Capacity> m_ts_generation_;   // [node][t]
};

// #pragma once

// #include <vector>
// #include "InjectedGrid.h"
// #include "TimeSeries.h"
// #include "GridSpec.h"
// #include "Types.h"  // NodeID, EdgeID, TimeStep, Power, Demand, Capacity, NodeType

// /**
//  * @file PowerGridSnapshot.h
//  * @brief Immutable, validated snapshot of a fully assembled power grid dataset.
//  *
//  * Combines topology (InjectedGrid), time series (TimeSeries),
//  * and metadata (GridSpec) into a single canonical, export-ready structure.
//  *
//  * This is the final in-memory representation before serialization (e.g. HDF5)
//  * or consumption by optimization solvers.
//  */
// class PowerGridSnapshot {
// public:
//     /**
//      * @brief Construct a fully validated, immutable snapshot.
//      *
//      * @param grid Immutable injected grid (topology + static attributes)
//      * @param ts Time series data aligned to grid nodes
//      * @param spec Grid-level metadata/specification
//      *
//      * @throws std::runtime_error if alignment or consistency checks fail
//      */
//     PowerGridSnapshot(const InjectedGrid& grid,
//                       const TimeSeries& ts,
//                       const GridSpec& spec);

//     // =========================
//     // Node Accessors
//     // =========================
//     size_t num_nodes() const { return m_num_nodes_; }
//     const std::vector<NodeID>& node_ids() const { return m_node_ids_; }
//     const std::vector<NodeType>& node_types() const { return m_node_types_; }
//     const std::vector<Power>& node_base_load() const { return m_node_base_load_; }
//     const std::vector<Capacity>& node_max_capacity() const { return m_node_max_capacity_; }

//     // =========================
//     // Edge Accessors
//     // =========================
//     size_t num_edges() const { return m_num_edges_; }
//     const std::vector<EdgeID>& edge_ids() const { return m_edge_ids_; }
//     const std::vector<NodeID>& edge_from() const { return m_edge_from_; }
//     const std::vector<NodeID>& edge_to() const { return m_edge_to_; }
//     const std::vector<Power>& edge_reactance() const { return m_edge_reactance_; }
//     const std::vector<Capacity>& edge_max_capacity() const { return m_edge_max_capacity_; }

//     // =========================
//     // Time Series Accessors
//     // =========================
//     size_t num_timesteps() const { return m_num_timesteps_; }
//     const std::vector<Demand>& ts_demand() const { return m_ts_demand_; }        // flattened [node][t]
//     const std::vector<Capacity>& ts_generation() const { return m_ts_generation_; } // flattened [node][t]

//     // =========================
//     // Grid Specification
//     // =========================
//     const GridSpec& spec() const { return m_spec_; }

// private:
//     // Validation helper
//     void validate(const InjectedGrid& grid, const TimeSeries& ts) const;

//     // Metadata
//     GridSpec m_spec_;
//     size_t m_num_nodes_;
//     size_t m_num_edges_;
//     size_t m_num_timesteps_;

//     // Node-level data
//     std::vector<NodeID> m_node_ids_;
//     std::vector<NodeType> m_node_types_;
//     std::vector<Power> m_node_base_load_;
//     std::vector<Capacity> m_node_max_capacity_;

//     // Edge-level data
//     std::vector<EdgeID> m_edge_ids_;
//     std::vector<NodeID> m_edge_from_;
//     std::vector<NodeID> m_edge_to_;
//     std::vector<Power> m_edge_reactance_;
//     std::vector<Capacity> m_edge_max_capacity_;

//     // Flattened time series
//     std::vector<Demand> m_ts_demand_;        // [node][t]
//     std::vector<Capacity> m_ts_generation_;  // [node][t]
// };
