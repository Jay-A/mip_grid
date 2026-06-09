// tests/unit/test_grid_dataset_exporter.cpp

#include "GridBuilder.h"
#include "GridInjector.h"
#include "GridDatasetExporter.h"
#include "TimeSeries.h"
#include "hdf5.h"

#include <cassert>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>

int main() {
    // ----------------------------
    // Build a minimal grid
    // ----------------------------
    GridBuilder builder;

    NodeID n0 = builder.add_node(); // Load
    NodeID n1 = builder.add_node(); // Generator

    builder.add_edge(n0, n1);

    Grid grid = std::move(builder).build(); // must move builder

    // ----------------------------
    // Inject attributes
    // ----------------------------
    GridInjector injector;

    GridSpec spec; // minimal/default spec

    InjectedGrid injected = injector.inject(grid, spec);

    // Optional: assign realistic attributes if API allows
    // injected.node(n0).base_load = 10.0;   // pseudo-code
    // injected.node(n1).max_capacity = 50.0;

    // ----------------------------
    // Create a minimal time series
    // ----------------------------
    const std::size_t T = 3;
    std::vector<std::vector<double>> demand(grid.num_nodes(), std::vector<double>(T, 1.0));
    std::vector<std::vector<double>> generation(grid.num_nodes(), std::vector<double>(T, 2.0));

    TimeSeries ts(injected, std::move(demand), std::move(generation));

    // ----------------------------
    // Export to HDF5
    // ----------------------------
    std::string filename = "/tmp/test_grid_export.h5";

    GridDatasetExporter exporter(injected, &ts);

    try {
        exporter.export_hdf5(filename);
        std::cout << "Export succeeded: " << filename << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Export failed: " << e.what() << "\n";
        return 1;
    }

    // ----------------------------
    // Basic sanity check
    // ----------------------------
    FILE* f = std::fopen(filename.c_str(), "r");
    assert(f != nullptr && "HDF5 file was not created!");
    std::fclose(f);

    // Cleanup (optional)
    std::remove(filename.c_str());

    std::cout << "Unit test passed.\n";
    return 0;
}