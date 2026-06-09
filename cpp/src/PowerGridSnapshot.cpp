#include "PowerGridSnapshot.h"

#include <stdexcept>

// ============================================================
// Constructor
// ============================================================

// PowerGridSnapshot::PowerGridSnapshot(
//     const InjectedGrid& grid,
//     const TimeSeries& ts,
//     const GridSpec& spec,
//     const TimeSeriesAttributes& ts_attr)
//     : m_spec_(spec),
//       m_num_nodes_(0),
//       m_num_edges_(0),
//       m_num_timesteps_(0)
PowerGridSnapshot::PowerGridSnapshot(
    const InjectedGrid& grid,
    const TimeSeries& ts,
    const GridSpec& spec,
    const TimeSeriesAttributes& ts_attr)
    : m_spec_(spec),
      m_ts_attr_(ts_attr),
      m_num_nodes_(0),
      m_num_edges_(0),
      m_num_timesteps_(ts_attr.timesteps)
{
    validate(grid, ts);

    m_num_nodes_ = grid.nodes().size();
    m_num_edges_ = grid.edges().size();
    // m_num_timesteps_ = ts.num_timesteps();
    m_num_timesteps_ = ts_attr.timesteps;

    // ========================================================
    // Nodes
    // ========================================================

    m_node_ids_.reserve(m_num_nodes_);
    m_node_types_.reserve(m_num_nodes_);
    m_node_base_load_.reserve(m_num_nodes_);
    m_node_max_capacity_.reserve(m_num_nodes_);

    for (NodeID i = 0; i < static_cast<NodeID>(m_num_nodes_); ++i) {
        const auto& node = grid.node(i);

        m_node_ids_.push_back(i);
        m_node_types_.push_back(node.type);
        m_node_base_load_.push_back(node.base_load);
        m_node_max_capacity_.push_back(node.max_capacity);
    }

    // ========================================================
    // Edges
    // ========================================================

    const auto& topology = grid.grid();

    m_edge_ids_.reserve(m_num_edges_);
    m_edge_from_.reserve(m_num_edges_);
    m_edge_to_.reserve(m_num_edges_);
    m_edge_reactance_.reserve(m_num_edges_);
    m_edge_max_capacity_.reserve(m_num_edges_);

    for (EdgeID i = 0; i < static_cast<EdgeID>(m_num_edges_); ++i) {
        const auto& edge_attr = grid.edges()[i];
        const auto& edge_topo = topology.edge(i);

        m_edge_ids_.push_back(i);
        m_edge_from_.push_back(edge_topo.from);
        m_edge_to_.push_back(edge_topo.to);
        m_edge_reactance_.push_back(edge_attr.reactance);
        m_edge_max_capacity_.push_back(edge_attr.max_capacity);
    }

    // ========================================================
    // Time Series
    // ========================================================

    m_ts_demand_.reserve(m_num_nodes_ * m_num_timesteps_);
    m_ts_generation_.reserve(m_num_nodes_ * m_num_timesteps_);

    for (NodeID i = 0; i < static_cast<NodeID>(m_num_nodes_); ++i) {

        const auto& demand = ts.demand(i);
        const auto& generation = ts.generation(i);

        m_ts_demand_.insert(
            m_ts_demand_.end(),
            demand.begin(),
            demand.end());

        m_ts_generation_.insert(
            m_ts_generation_.end(),
            generation.begin(),
            generation.end());
    }
}

// ============================================================
// Validation
// ============================================================

void PowerGridSnapshot::validate(
    const InjectedGrid& grid,
    const TimeSeries& ts) const
{
    const std::size_t node_count = grid.nodes().size();

    if (node_count != ts.num_nodes()) {
        throw std::runtime_error(
            "PowerGridSnapshot: InjectedGrid node count does not match "
            "TimeSeries node count.");
    }

    const std::size_t T = ts.num_timesteps();

    if (T == 0) {
        throw std::runtime_error(
            "PowerGridSnapshot: TimeSeries contains zero timesteps.");
    }

    for (NodeID i = 0; i < static_cast<NodeID>(node_count); ++i) {

        const auto& demand = ts.demand(i);
        const auto& generation = ts.generation(i);

        if (demand.size() != T) {
            throw std::runtime_error(
                "PowerGridSnapshot: inconsistent demand vector length.");
        }

        if (generation.size() != T) {
            throw std::runtime_error(
                "PowerGridSnapshot: inconsistent generation vector length.");
        }
    }
}