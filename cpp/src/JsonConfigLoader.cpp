#include "JsonConfigLoader.h"
#include "json.hpp" // nlohmann/json.hpp
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

void JsonConfigLoader::load(const std::string& filename, GridSpec& spec, TimeSeriesAttributes& ts_attr)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Unable to open JSON file: " + filename);

    json root;
    file >> root;

    if (!root.contains("grid") || !root.contains("timeseries"))
        throw std::runtime_error("JSON must contain 'grid' and 'timeseries' sections");

    const auto& grid = root["grid"];
    const auto& ts = root["timeseries"];

    // ----------------------
    // GridSpec
    // ----------------------
    spec.num_nodes = grid.at("num_nodes").get<size_t>();
    spec.seed = grid.at("seed").get<uint64_t>();
    spec.use_ring_backbone = grid.at("use_ring_backbone").get<bool>();
    spec.generator_ratio = grid.at("generator_ratio").get<double>();
    spec.load_ratio = grid.at("load_ratio").get<double>();
    spec.cross_link_rate = grid.at("cross_link_rate").get<double>();
    spec.branch_factor = grid.at("branch_factor").get<size_t>();

    if (grid.contains("require_connected_graph"))
        spec.require_connected_graph = grid["require_connected_graph"].get<bool>();
    if (grid.contains("require_generator_reachability"))
        spec.require_generator_reachability = grid["require_generator_reachability"].get<bool>();
    if (grid.contains("max_unserved_load_fraction"))
        spec.max_unserved_load_fraction = grid["max_unserved_load_fraction"].get<double>();

    if (grid.contains("generator_capacity_min"))
        spec.generator_capacity_min = grid["generator_capacity_min"].get<double>();
    if (grid.contains("generator_capacity_max"))
        spec.generator_capacity_max = grid["generator_capacity_max"].get<double>();
    if (grid.contains("load_demand_min"))
        spec.load_demand_min = grid["load_demand_min"].get<double>();
    if (grid.contains("load_demand_max"))
        spec.load_demand_max = grid["load_demand_max"].get<double>();
    if (grid.contains("edge_capacity_min"))
        spec.edge_capacity_min = grid["edge_capacity_min"].get<double>();
    if (grid.contains("edge_capacity_max"))
        spec.edge_capacity_max = grid["edge_capacity_max"].get<double>();
    if (grid.contains("edge_reactance_min"))
        spec.edge_reactance_min = grid["edge_reactance_min"].get<double>();
    if (grid.contains("edge_reactance_max"))
        spec.edge_reactance_max = grid["edge_reactance_max"].get<double>();
    if (grid.contains("node_failure_prob"))
        spec.node_failure_prob = grid["node_failure_prob"].get<double>();
    if (grid.contains("edge_failure_prob"))
        spec.edge_failure_prob = grid["edge_failure_prob"].get<double>();

    // ----------------------
    // TimeSeriesAttributes
    // ----------------------
    ts_attr.timesteps = ts.at("timesteps").get<size_t>();
    ts_attr.sample_frequency_minutes = ts.at("sample_frequency_minutes").get<size_t>();
    ts_attr.demand_modulation_amplitude = ts.at("demand_modulation_amplitude").get<double>();
    ts_attr.generator_modulation_amplitude = ts.at("generator_modulation_amplitude").get<double>();
    ts_attr.noise_amplitude = ts.at("noise_amplitude").get<double>();
    ts_attr.seed = ts.at("seed").get<uint64_t>();
    ts_attr.label = ts.contains("label") ? ts["label"].get<std::string>() : std::string{};
}
