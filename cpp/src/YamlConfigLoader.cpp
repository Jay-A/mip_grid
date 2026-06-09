#include "YamlConfigLoader.h"
#include "Types.h"
#include "GridSpec.h"

#include <yaml-cpp/yaml.h>
#include <stdexcept>

static void require(const YAML::Node& node, const std::string& key)
{
    if (!node[key])
        throw std::runtime_error("Missing required YAML field: " + key);
}

void YamlConfigLoader::load(
    const std::string& filename,
    GridSpec& spec,
    TimeSeriesAttributes& ts_attr)
{
    YAML::Node root = YAML::LoadFile(filename);

    if (!root["grid"] || !root["timeseries"])
        throw std::runtime_error("YAML must contain 'grid' and 'timeseries' sections");

    const YAML::Node grid = root["grid"];
    const YAML::Node ts   = root["timeseries"];

    // ============================================================
    // GridSpec (required fields first)
    // ============================================================

    require(grid, "num_nodes");
    require(grid, "seed");
    require(grid, "generator_ratio");
    require(grid, "load_ratio");
    require(grid, "branch_factor");

    spec.num_nodes = grid["num_nodes"].as<size_t>();
    spec.seed = grid["seed"].as<uint64_t>();
    spec.generator_ratio = grid["generator_ratio"].as<double>();
    spec.load_ratio = grid["load_ratio"].as<double>();
    spec.branch_factor = grid["branch_factor"].as<size_t>();

    // Optional fields
    if (grid["use_ring_backbone"])
        spec.use_ring_backbone = grid["use_ring_backbone"].as<bool>();

    if (grid["add_cross_links"])
        spec.add_cross_links = grid["add_cross_links"].as<bool>();

    if (grid["cross_link_rate"])
        spec.cross_link_rate = grid["cross_link_rate"].as<double>();

    if (grid["require_connected_graph"])
        spec.require_connected_graph = grid["require_connected_graph"].as<bool>();

    if (grid["require_generator_reachability"])
        spec.require_generator_reachability = grid["require_generator_reachability"].as<bool>();

    if (grid["max_unserved_load_fraction"])
        spec.max_unserved_load_fraction = grid["max_unserved_load_fraction"].as<double>();

    // Physics (optional block, but usually present)
    if (grid["generator_capacity_min"])
        spec.generator_capacity_min = grid["generator_capacity_min"].as<double>();
    if (grid["generator_capacity_max"])
        spec.generator_capacity_max = grid["generator_capacity_max"].as<double>();
    if (grid["load_demand_min"])
        spec.load_demand_min = grid["load_demand_min"].as<double>();
    if (grid["load_demand_max"])
        spec.load_demand_max = grid["load_demand_max"].as<double>();

    if (grid["edge_capacity_min"])
        spec.edge_capacity_min = grid["edge_capacity_min"].as<double>();
    if (grid["edge_capacity_max"])
        spec.edge_capacity_max = grid["edge_capacity_max"].as<double>();

    if (grid["edge_reactance_min"])
        spec.edge_reactance_min = grid["edge_reactance_min"].as<double>();
    if (grid["edge_reactance_max"])
        spec.edge_reactance_max = grid["edge_reactance_max"].as<double>();

    if (grid["node_failure_prob"])
        spec.node_failure_prob = grid["node_failure_prob"].as<double>();
    if (grid["edge_failure_prob"])
        spec.edge_failure_prob = grid["edge_failure_prob"].as<double>();

    // ============================================================
    // TimeSeriesAttributes
    // ============================================================

    require(ts, "timesteps");
    require(ts, "sample_frequency_minutes");
    require(ts, "demand_modulation_amplitude");
    require(ts, "generator_modulation_amplitude");
    require(ts, "noise_amplitude");
    require(ts, "seed");

    ts_attr.timesteps = ts["timesteps"].as<std::size_t>();
    ts_attr.sample_frequency_minutes = ts["sample_frequency_minutes"].as<std::size_t>();
    ts_attr.demand_modulation_amplitude = ts["demand_modulation_amplitude"].as<double>();
    ts_attr.generator_modulation_amplitude = ts["generator_modulation_amplitude"].as<double>();
    ts_attr.noise_amplitude = ts["noise_amplitude"].as<double>();
    ts_attr.seed = ts["seed"].as<uint64_t>();

    if (ts["label"])
        ts_attr.label = ts["label"].as<std::string>();
    else
        ts_attr.label.clear();
}