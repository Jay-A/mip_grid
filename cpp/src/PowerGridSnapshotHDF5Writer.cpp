#include "PowerGridSnapshotHDF5Writer.h"
#include <vector>
#include <stdexcept>

PowerGridSnapshotHDF5Writer::PowerGridSnapshotHDF5Writer(
    const PowerGridSnapshot& snapshot)
    : m_snapshot_(snapshot)
{ }

void PowerGridSnapshotHDF5Writer::write(const std::string& filename) const {
    H5::H5File file(filename, H5F_ACC_TRUNC);

    write_meta(file);
    write_nodes(file);
    write_edges(file);
    write_time_series(file);
}

// -----------------------------
// /meta
// -----------------------------
void PowerGridSnapshotHDF5Writer::write_meta(H5::H5File& file) const {
    const GridSpec& spec = m_snapshot_.spec();

    file.createGroup("/meta");

    // store simple scalars in locals to avoid taking & of temporaries
    const hsize_t num_nodes = m_snapshot_.num_nodes();
    const hsize_t num_edges = m_snapshot_.num_edges();
    const hsize_t num_timesteps = m_snapshot_.num_timesteps();

    file.createDataSet("/meta/num_nodes", H5::PredType::NATIVE_HSIZE,
                       H5::DataSpace()).write(&num_nodes, H5::PredType::NATIVE_HSIZE);
    file.createDataSet("/meta/num_edges", H5::PredType::NATIVE_HSIZE,
                       H5::DataSpace()).write(&num_edges, H5::PredType::NATIVE_HSIZE);
    file.createDataSet("/meta/num_timesteps", H5::PredType::NATIVE_HSIZE,
                       H5::DataSpace()).write(&num_timesteps, H5::PredType::NATIVE_HSIZE);

    // Helper to convert bool to int8_t
    auto bool_to_int8 = [](bool b) -> int8_t { return b ? 1 : 0; };
    int8_t tmp_bool;

    // Write all remaining fields from GridSpec
    file.createDataSet("/meta/generator_ratio", H5::PredType::NATIVE_DOUBLE, H5::DataSpace())
        .write(&spec.generator_ratio, H5::PredType::NATIVE_DOUBLE);
    file.createDataSet("/meta/load_ratio", H5::PredType::NATIVE_DOUBLE, H5::DataSpace())
        .write(&spec.load_ratio, H5::PredType::NATIVE_DOUBLE);

    tmp_bool = bool_to_int8(spec.require_connected_graph);
    file.createDataSet("/meta/require_connected_graph", H5::PredType::NATIVE_INT8, H5::DataSpace())
        .write(&tmp_bool, H5::PredType::NATIVE_INT8);

    tmp_bool = bool_to_int8(spec.require_generator_reachability);
    file.createDataSet("/meta/require_generator_reachability", H5::PredType::NATIVE_INT8, H5::DataSpace())
        .write(&tmp_bool, H5::PredType::NATIVE_INT8);

    file.createDataSet("/meta/max_unserved_load_fraction", H5::PredType::NATIVE_DOUBLE, H5::DataSpace())
        .write(&spec.max_unserved_load_fraction, H5::PredType::NATIVE_DOUBLE);

    file.createDataSet("/meta/branch_factor", H5::PredType::NATIVE_HSIZE, H5::DataSpace())
        .write(&spec.branch_factor, H5::PredType::NATIVE_HSIZE);

    tmp_bool = bool_to_int8(spec.use_ring_backbone);
    file.createDataSet("/meta/use_ring_backbone", H5::PredType::NATIVE_INT8, H5::DataSpace())
        .write(&tmp_bool, H5::PredType::NATIVE_INT8);

    tmp_bool = bool_to_int8(spec.add_cross_links);
    file.createDataSet("/meta/add_cross_links", H5::PredType::NATIVE_INT8, H5::DataSpace())
        .write(&tmp_bool, H5::PredType::NATIVE_INT8);

    file.createDataSet("/meta/cross_link_rate", H5::PredType::NATIVE_DOUBLE, H5::DataSpace())
        .write(&spec.cross_link_rate, H5::PredType::NATIVE_DOUBLE);
    file.createDataSet("/meta/seed", H5::PredType::NATIVE_UINT32, H5::DataSpace())
        .write(&spec.seed, H5::PredType::NATIVE_UINT32);

    file.createDataSet("/meta/generator_capacity_min", H5::PredType::NATIVE_DOUBLE, H5::DataSpace())
        .write(&spec.generator_capacity_min, H5::PredType::NATIVE_DOUBLE);
    file.createDataSet("/meta/generator_capacity_max", H5::PredType::NATIVE_DOUBLE, H5::DataSpace())
        .write(&spec.generator_capacity_max, H5::PredType::NATIVE_DOUBLE);
    file.createDataSet("/meta/load_demand_min", H5::PredType::NATIVE_DOUBLE, H5::DataSpace())
        .write(&spec.load_demand_min, H5::PredType::NATIVE_DOUBLE);
    file.createDataSet("/meta/load_demand_max", H5::PredType::NATIVE_DOUBLE, H5::DataSpace())
        .write(&spec.load_demand_max, H5::PredType::NATIVE_DOUBLE);

    file.createDataSet("/meta/edge_capacity_min", H5::PredType::NATIVE_DOUBLE, H5::DataSpace())
        .write(&spec.edge_capacity_min, H5::PredType::NATIVE_DOUBLE);
    file.createDataSet("/meta/edge_capacity_max", H5::PredType::NATIVE_DOUBLE, H5::DataSpace())
        .write(&spec.edge_capacity_max, H5::PredType::NATIVE_DOUBLE);
    file.createDataSet("/meta/edge_reactance_min", H5::PredType::NATIVE_DOUBLE, H5::DataSpace())
        .write(&spec.edge_reactance_min, H5::PredType::NATIVE_DOUBLE);
    file.createDataSet("/meta/edge_reactance_max", H5::PredType::NATIVE_DOUBLE, H5::DataSpace())
        .write(&spec.edge_reactance_max, H5::PredType::NATIVE_DOUBLE);

    file.createDataSet("/meta/node_failure_prob", H5::PredType::NATIVE_DOUBLE, H5::DataSpace())
        .write(&spec.node_failure_prob, H5::PredType::NATIVE_DOUBLE);
    file.createDataSet("/meta/edge_failure_prob", H5::PredType::NATIVE_DOUBLE, H5::DataSpace())
        .write(&spec.edge_failure_prob, H5::PredType::NATIVE_DOUBLE);

    const auto& ts_attr = m_snapshot_.ts_attributes();

    // TimeSeries configuration
    file.createDataSet("/meta/timesteps", H5::PredType::NATIVE_HSIZE, H5::DataSpace())
        .write(&ts_attr.timesteps, H5::PredType::NATIVE_HSIZE);
    file.createDataSet("/meta/sample_frequency_minutes", H5::PredType::NATIVE_HSIZE, H5::DataSpace())
        .write(&ts_attr.sample_frequency_minutes, H5::PredType::NATIVE_HSIZE);
    file.createDataSet("/meta/demand_modulation_amplitude", H5::PredType::NATIVE_DOUBLE, H5::DataSpace())
        .write(&ts_attr.demand_modulation_amplitude, H5::PredType::NATIVE_DOUBLE);
    file.createDataSet("/meta/generator_modulation_amplitude", H5::PredType::NATIVE_DOUBLE, H5::DataSpace())
        .write(&ts_attr.generator_modulation_amplitude, H5::PredType::NATIVE_DOUBLE);
    file.createDataSet("/meta/noise_amplitude", H5::PredType::NATIVE_DOUBLE, H5::DataSpace())
        .write(&ts_attr.noise_amplitude, H5::PredType::NATIVE_DOUBLE);
    file.createDataSet("/meta/seed_timeseries", H5::PredType::NATIVE_UINT32, H5::DataSpace())
        .write(&ts_attr.seed, H5::PredType::NATIVE_UINT32);

    // store label as fixed-length string or variable-length string
    H5::StrType str_type(H5::PredType::C_S1, H5T_VARIABLE);
    file.createDataSet("/meta/label", str_type, H5::DataSpace())
        .write(ts_attr.label, str_type);
}

void PowerGridSnapshotHDF5Writer::write_nodes(H5::H5File& file) const {
    file.createGroup("/nodes");

    const hsize_t n = static_cast<hsize_t>(m_snapshot_.num_nodes());

    const auto& ids = m_snapshot_.node_ids();
    const auto& types = m_snapshot_.node_types();
    const auto& base_load = m_snapshot_.node_base_load();
    const auto& max_capacity = m_snapshot_.node_max_capacity();

    if (ids.size() != n || types.size() != n ||
        base_load.size() != n || max_capacity.size() != n) {
        throw std::runtime_error("PowerGridSnapshotHDF5Writer: node array size mismatch");
    }

    auto write_1d = [&](const std::string& path, const auto& data, auto type) {
        H5::DataSpace space(1, &n);
        H5::DataSet ds = file.createDataSet(path, type, space);
        ds.write(data.data(), type);
    };

    write_1d("/nodes/id", ids, H5::PredType::NATIVE_UINT32);
    write_1d("/nodes/type", types, H5::PredType::NATIVE_INT8);
    write_1d("/nodes/base_load", base_load, H5::PredType::NATIVE_DOUBLE);
    write_1d("/nodes/max_capacity", max_capacity, H5::PredType::NATIVE_DOUBLE);
}

void PowerGridSnapshotHDF5Writer::write_edges(H5::H5File& file) const {
    file.createGroup("/edges");

    const hsize_t n = static_cast<hsize_t>(m_snapshot_.num_edges());

    const auto& ids = m_snapshot_.edge_ids();
    const auto& from = m_snapshot_.edge_from();
    const auto& to = m_snapshot_.edge_to();
    const auto& reactance = m_snapshot_.edge_reactance();
    const auto& max_capacity = m_snapshot_.edge_max_capacity();

    if (ids.size() != n || from.size() != n || to.size() != n ||
        reactance.size() != n || max_capacity.size() != n) {
        throw std::runtime_error("PowerGridSnapshotHDF5Writer: edge array size mismatch");
    }

    auto write_1d = [&](const std::string& path, const auto& data, auto type) {
        H5::DataSpace space(1, &n);
        H5::DataSet ds = file.createDataSet(path, type, space);
        ds.write(data.data(), type);
    };

    write_1d("/edges/id", ids, H5::PredType::NATIVE_UINT32);
    write_1d("/edges/from", from, H5::PredType::NATIVE_UINT32);
    write_1d("/edges/to", to, H5::PredType::NATIVE_UINT32);
    write_1d("/edges/reactance", reactance, H5::PredType::NATIVE_DOUBLE);
    write_1d("/edges/max_capacity", max_capacity, H5::PredType::NATIVE_DOUBLE);
}

void PowerGridSnapshotHDF5Writer::write_time_series(H5::H5File& file) const {
    file.createGroup("/time_series");

    const hsize_t N = static_cast<hsize_t>(m_snapshot_.num_nodes());
    const hsize_t T = static_cast<hsize_t>(m_snapshot_.num_timesteps());

    // Helper to write 2D arrays
    auto write_2d = [&](const std::string& path, const auto& data, auto type) {
        hsize_t dims[2] = {N, T};
        H5::DataSpace space(2, dims);
        H5::DataSet ds = file.createDataSet(path, type, space);
        ds.write(data.data(), type);
    };

    write_2d("/time_series/demand", m_snapshot_.ts_demand(), H5::PredType::NATIVE_DOUBLE);
    write_2d("/time_series/generation", m_snapshot_.ts_generation(), H5::PredType::NATIVE_DOUBLE);
}