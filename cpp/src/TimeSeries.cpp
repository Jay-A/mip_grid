#include "TimeSeries.h"

#include <stdexcept>
#include <cstddef>

TimeSeries::TimeSeries(const InjectedGrid& grid,
                       std::vector<std::vector<double>> demand_time_series,
                       std::vector<std::vector<double>> generation_time_series)
    : m_demand(std::move(demand_time_series)),
      m_generation(std::move(generation_time_series)),
      m_num_nodes(grid.nodes().size()),
      m_num_timesteps(m_demand.empty() ? 0 : m_demand[0].size()) {

    // Validate node dimension
    if (m_demand.size() != m_num_nodes || m_generation.size() != m_num_nodes) {
        throw std::runtime_error("TimeSeries: Node count mismatch with InjectedGrid");
    }

    // Validate timestep dimension
    for (std::size_t i = 0; i < m_num_nodes; ++i) {
        if (m_demand[i].size() != m_num_timesteps) {
            throw std::runtime_error("TimeSeries: Inconsistent demand timesteps for node " + std::to_string(i));
        }
        if (m_generation[i].size() != m_num_timesteps) {
            throw std::runtime_error("TimeSeries: Inconsistent generation timesteps for node " + std::to_string(i));
        }
    }
}

std::size_t TimeSeries::num_nodes() const noexcept {
    return m_num_nodes;
}

std::size_t TimeSeries::num_timesteps() const noexcept {
    return m_num_timesteps;
}

const std::vector<double>& TimeSeries::demand(NodeID node_id) const {
    if (node_id >= m_num_nodes) {
        throw std::out_of_range("TimeSeries::demand() node_id out of range");
    }
    return m_demand[node_id];
}

const std::vector<double>& TimeSeries::generation(NodeID node_id) const {
    if (node_id >= m_num_nodes) {
        throw std::out_of_range("TimeSeries::generation() node_id out of range");
    }
    return m_generation[node_id];
}

double TimeSeries::demand(NodeID node_id, TimeStep t) const {
    if (node_id >= m_num_nodes || t >= m_num_timesteps) {
        throw std::out_of_range("TimeSeries::demand() index out of range");
    }
    return m_demand[node_id][t];
}

double TimeSeries::generation(NodeID node_id, std::size_t t) const {
    if (node_id >= m_num_nodes || t >= m_num_timesteps) {
        throw std::out_of_range("TimeSeries::generation() index out of range");
    }
    return m_generation[node_id][t];
}

const std::vector<std::vector<double>>& TimeSeries::demand_matrix() const noexcept {
    return m_demand;
}

const std::vector<std::vector<double>>& TimeSeries::generation_matrix() const noexcept {
    return m_generation;
}
