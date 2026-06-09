// test_json_config_loader.cpp
#include "JsonConfigLoader.h"
#include "GridSpec.h"
#include "TimeSeriesAttributes.h"

#include <cassert>
#include <iostream>
#include <fstream>

int main()
{
    // ----------------------------
    // 1. Create a temporary JSON file
    // ----------------------------
    const char* filename = "test_config.json";
    std::ofstream ofs(filename);
    ofs << R"json(
{
  "grid": {
    "num_nodes": 10,
    "generator_ratio": 0.2,
    "load_ratio": 0.5,
    "require_connected_graph": true,
    "branch_factor": 3,
    "use_ring_backbone": true,
    "add_cross_links": false,
    "cross_link_rate": 0.1,
    "seed": 12345,
    "generator_capacity_min": 50.0,
    "generator_capacity_max": 300.0,
    "load_demand_min": 5.0,
    "load_demand_max": 50.0,
    "edge_capacity_min": 100.0,
    "edge_capacity_max": 500.0,
    "edge_reactance_min": 0.01,
    "edge_reactance_max": 0.05,
    "node_failure_prob": 0.01,
    "edge_failure_prob": 0.02
  },
  "timeseries": {
    "timesteps": 24,
    "sample_frequency_minutes": 60,
    "demand_modulation_amplitude": 0.2,
    "generator_modulation_amplitude": 0.1,
    "noise_amplitude": 0.05,
    "seed": 42,
    "label": "test_series"
  }
}
)json";
    ofs.close();

    // ----------------------------
    // 2. Load into structs
    // ----------------------------
    GridSpec spec;
    TimeSeriesAttributes ts_attr;

    JsonConfigLoader::load(filename, spec, ts_attr);

    // ----------------------------
    // 3. Check a few key fields
    // ----------------------------
    assert(spec.num_nodes == 10);
    assert(spec.generator_ratio == 0.2);
    assert(spec.load_ratio == 0.5);
    assert(spec.seed == 12345);
    assert(spec.use_ring_backbone == true);
    assert(ts_attr.timesteps == 24);
    assert(ts_attr.seed == 42);
    assert(ts_attr.label == "test_series");

    std::cout << "JsonConfigLoader test passed!\n";
    return 0;
}
