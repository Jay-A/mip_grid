#include "YamlConfigLoader.h"
#include "GridSpec.h"
#include "TimeSeriesAttributes.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdio>
#include <cassert>

bool approx(double a, double b, double eps = 1e-12) {
    return std::fabs(a - b) < eps;
}

int main() {
    const std::string filename = "test_config.yaml";

    // -----------------------------
    // 1. Write a temporary YAML file
    // -----------------------------
    const std::string yaml_content = R"(
grid:
  num_nodes: 10
  generator_ratio: 0.2
  load_ratio: 0.5
  require_connected_graph: true
  require_generator_reachability: true
  max_unserved_load_fraction: 0.0
  branch_factor: 3
  use_ring_backbone: true
  add_cross_links: false
  cross_link_rate: 0.1
  seed: 12345
  generator_capacity_min: 50.0
  generator_capacity_max: 300.0
  load_demand_min: 5.0
  load_demand_max: 50.0
  edge_capacity_min: 100.0
  edge_capacity_max: 500.0
  edge_reactance_min: 0.01
  edge_reactance_max: 0.05
  node_failure_prob: 0.01
  edge_failure_prob: 0.02

timeseries:
  timesteps: 24
  sample_frequency_minutes: 60
  demand_modulation_amplitude: 0.2
  generator_modulation_amplitude: 0.1
  noise_amplitude: 0.05
  seed: 42
  label: "test_series"
)";

    std::ofstream fout(filename);
    if (!fout) {
        std::cerr << "Error creating YAML file.\n";
        return 1;
    }
    fout << yaml_content;
    fout.close();

    // -----------------------------
    // 2. Load YAML
    // -----------------------------
    GridSpec spec;
    TimeSeriesAttributes ts_attr;

    try {
        YamlConfigLoader::load(filename, spec, ts_attr);
    } catch (const std::exception& e) {
        std::cerr << "YamlConfigLoader failed: " << e.what() << "\n";
        return 1;
    }

    // -----------------------------
    // 3. Verify GridSpec
    // -----------------------------
    if (spec.num_nodes != 10) return 1;
    if (!approx(spec.generator_ratio, 0.2)) return 1;
    if (!approx(spec.load_ratio, 0.5)) return 1;
    if (!spec.require_connected_graph) return 1;
    if (!spec.require_generator_reachability) return 1;
    if (!approx(spec.max_unserved_load_fraction, 0.0)) return 1;
    if (spec.branch_factor != 3) return 1;
    if (!spec.use_ring_backbone) return 1;
    if (spec.add_cross_links) return 1;
    if (!approx(spec.cross_link_rate, 0.1)) return 1;
    if (spec.seed != 12345) return 1;

    if (!approx(spec.generator_capacity_min, 50.0)) return 1;
    if (!approx(spec.generator_capacity_max, 300.0)) return 1;
    if (!approx(spec.load_demand_min, 5.0)) return 1;
    if (!approx(spec.load_demand_max, 50.0)) return 1;
    if (!approx(spec.edge_capacity_min, 100.0)) return 1;
    if (!approx(spec.edge_capacity_max, 500.0)) return 1;
    if (!approx(spec.edge_reactance_min, 0.01)) return 1;
    if (!approx(spec.edge_reactance_max, 0.05)) return 1;
    if (!approx(spec.node_failure_prob, 0.01)) return 1;
    if (!approx(spec.edge_failure_prob, 0.02)) return 1;

    // -----------------------------
    // 4. Verify TimeSeriesAttributes
    // -----------------------------
    if (ts_attr.timesteps != 24) return 1;
    if (ts_attr.sample_frequency_minutes != 60) return 1;
    if (!approx(ts_attr.demand_modulation_amplitude, 0.2)) return 1;
    if (!approx(ts_attr.generator_modulation_amplitude, 0.1)) return 1;
    if (!approx(ts_attr.noise_amplitude, 0.05)) return 1;
    if (ts_attr.seed != 42) return 1;
    if (ts_attr.label != "test_series") return 1;

    // -----------------------------
    // 5. Clean up
    // -----------------------------
    std::remove(filename.c_str());

    std::cout << "YamlConfigLoader basic unit test passed.\n";
    return 0;
}
