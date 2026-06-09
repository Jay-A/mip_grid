#include "test_utils.h"
#include "PowerGridSnapshotHDF5Writer.h"

#include <H5Cpp.h>
#include <cassert>
#include <iostream>

int main()
{
    const std::string filename = "test_snapshot.h5";

    // ----------------------------
    // Build deterministic snapshot inputs
    // ----------------------------
    GridSpec spec = GridSpec::Minimal(6);

    InjectedGrid grid = make_test_injected_grid(spec.num_nodes, 10);
    TimeSeries ts = make_test_timeseries(grid, 12);

    // ----------------------------
    // NEW: TimeSeriesAttributes (required by snapshot)
    // ----------------------------
    TimeSeriesAttributes ts_attr{};
    ts_attr.timesteps = ts.num_timesteps();              // must match TimeSeries
    ts_attr.sample_frequency_minutes = 60;
    ts_attr.demand_modulation_amplitude = 0.1;
    ts_attr.generator_modulation_amplitude = 0.0;
    ts_attr.noise_amplitude = 0.0;
    ts_attr.seed = 42;
    ts_attr.label = "unit_test";

    // ----------------------------
    // Construct snapshot (UPDATED SIGNATURE)
    // ----------------------------
    PowerGridSnapshot snapshot(grid, ts, spec, ts_attr);

    // ----------------------------
    // Write HDF5
    // ----------------------------
    PowerGridSnapshotHDF5Writer writer(snapshot);
    writer.write(filename);

    // ----------------------------
    // Open file for inspection
    // ----------------------------
    H5::H5File file(filename, H5F_ACC_RDONLY);

    auto read_hsize = [&](const std::string& path) -> hsize_t {
        H5::DataSet ds = file.openDataSet(path);
        hsize_t value;
        ds.read(&value, H5::PredType::NATIVE_HSIZE);
        return value;
    };

    auto read_double = [&](const std::string& path) -> double {
        H5::DataSet ds = file.openDataSet(path);
        double value;
        ds.read(&value, H5::PredType::NATIVE_DOUBLE);
        return value;
    };

    // ----------------------------
    // Assertions: structure + values
    // ----------------------------
    assert(read_hsize("/meta/num_nodes") == 6);
    assert(read_hsize("/meta/num_edges") == snapshot.num_edges());
    assert(read_hsize("/meta/num_timesteps") == 12);

    assert(read_double("/meta/generator_ratio") == spec.generator_ratio);
    assert(read_double("/meta/load_ratio") == spec.load_ratio);
    assert(read_double("/meta/cross_link_rate") == spec.cross_link_rate);

    // NEW: optional but now available in file
    assert(read_hsize("/meta/timesteps") == 12);

    std::cout << "HDF5 snapshot writer test passed.\n";

    return 0;
}

// #include "test_utils.h"
// #include "PowerGridSnapshotHDF5Writer.h"

// #include <H5Cpp.h>
// #include <cassert>
// #include <iostream>

// int main()
// {
//     const std::string filename = "test_snapshot.h5";

//     // ----------------------------
//     // Build deterministic snapshot
//     // ----------------------------
//     GridSpec spec = GridSpec::Minimal(6);

//     InjectedGrid grid = make_test_injected_grid(spec.num_nodes, 10);
//     TimeSeries ts = make_test_timeseries(grid, 12);

//     PowerGridSnapshot snapshot(grid, ts, spec);

//     // ----------------------------
//     // Write HDF5
//     // ----------------------------
//     PowerGridSnapshotHDF5Writer writer(snapshot);
//     writer.write(filename);

//     // ----------------------------
//     // Open file for inspection
//     // ----------------------------
//     H5::H5File file(filename, H5F_ACC_RDONLY);

//     auto read_hsize = [&](const std::string& path) -> hsize_t {
//         H5::DataSet ds = file.openDataSet(path);
//         hsize_t value;
//         ds.read(&value, H5::PredType::NATIVE_HSIZE);
//         return value;
//     };

//     auto read_double = [&](const std::string& path) -> double {
//         H5::DataSet ds = file.openDataSet(path);
//         double value;
//         ds.read(&value, H5::PredType::NATIVE_DOUBLE);
//         return value;
//     };

//     // ----------------------------
//     // Assertions: structure + values
//     // ----------------------------

//     assert(read_hsize("/meta/num_nodes") == 6);
//     assert(read_hsize("/meta/num_edges") == snapshot.num_edges());
//     assert(read_hsize("/meta/num_timesteps") == 12);

//     assert(read_double("/meta/generator_ratio") == spec.generator_ratio);
//     assert(read_double("/meta/load_ratio") == spec.load_ratio);
//     assert(read_double("/meta/cross_link_rate") == spec.cross_link_rate);

//     std::cout << "HDF5 snapshot writer test passed.\n";

//     return 0;
// }
