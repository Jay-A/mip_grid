#include "GridDatasetExporter.h"

#include <stdexcept>
#include <vector>
#include <string>
#include "hdf5.h"

// ============================================================
// Constructor
// ============================================================

GridDatasetExporter::GridDatasetExporter(const InjectedGrid& grid,
                                         const TimeSeries* ts)
    : m_grid(grid),
      m_timeseries(ts)
{}

// ============================================================
// Public API
// ============================================================

void GridDatasetExporter::export_hdf5(const std::string& filename) const {
    hid_t file_id = H5Fcreate(filename.c_str(),
                              H5F_ACC_TRUNC,
                              H5P_DEFAULT,
                              H5P_DEFAULT);

    if (file_id < 0) {
        throw std::runtime_error("Failed to create HDF5 file: " + filename);
    }

    write_nodes_hdf5(file_id);
    write_edges_hdf5(file_id);

    if (m_timeseries != nullptr) {
        write_timeseries_hdf5(file_id);
    }

    H5Fclose(file_id);
}

// ============================================================
// Nodes
// ============================================================

void GridDatasetExporter::write_nodes_hdf5(hid_t file_id) const {
    const auto& nodes = m_grid.nodes();
    const size_t N = nodes.size();

    std::vector<NodeID> ids(N);
    std::vector<int> types(N);
    std::vector<double> base_load(N);
    std::vector<double> max_capacity(N);

    for (size_t i = 0; i < N; ++i) {
        const auto& node = m_grid.node(static_cast<NodeID>(i));
        ids[i] = static_cast<NodeID>(i);
        types[i] = static_cast<int>(node.type);
        base_load[i] = node.base_load;
        max_capacity[i] = node.max_capacity;
    }

    hid_t space = H5Screate_simple(1, (hsize_t*)&N, nullptr);

    hid_t dset_ids = H5Dcreate(file_id, "/nodes/id", H5T_NATIVE_UINT32,
                               space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    hid_t dset_types = H5Dcreate(file_id, "/nodes/type", H5T_NATIVE_INT,
                                 space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    hid_t dset_load = H5Dcreate(file_id, "/nodes/base_load", H5T_NATIVE_DOUBLE,
                                space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    hid_t dset_cap = H5Dcreate(file_id, "/nodes/max_capacity", H5T_NATIVE_DOUBLE,
                               space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    H5Dwrite(dset_ids, H5T_NATIVE_UINT32, H5S_ALL, H5S_ALL, H5P_DEFAULT, ids.data());
    H5Dwrite(dset_types, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, types.data());
    H5Dwrite(dset_load, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, base_load.data());
    H5Dwrite(dset_cap, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, max_capacity.data());

    H5Dclose(dset_ids);
    H5Dclose(dset_types);
    H5Dclose(dset_load);
    H5Dclose(dset_cap);
    H5Sclose(space);
}

// ============================================================
// Edges
// ============================================================

void GridDatasetExporter::write_edges_hdf5(hid_t file_id) const {
    const auto& grid = m_grid.grid();         // underlying Grid
    const auto& edges = m_grid.edges();       // injected attributes
    const size_t E = edges.size();

    std::vector<EdgeID> ids(E);
    std::vector<NodeID> from(E);
    std::vector<NodeID> to(E);
    std::vector<double> reactance(E);
    std::vector<double> capacity(E);

    for (EdgeID i = 0; i < E; ++i) {
        const auto& edge_attr = edges[i];
        const auto& edge_topo = grid.edge(i);  // Edge topology from Grid

        ids[i] = i;
        from[i] = edge_topo.from;              // from Grid
        to[i]   = edge_topo.to;                // from Grid
        reactance[i] = edge_attr.reactance;    // injected attribute
        capacity[i]  = edge_attr.max_capacity; // injected attribute
    }

    hsize_t dims[1] = { static_cast<hsize_t>(E) };
    hid_t space = H5Screate_simple(1, dims, nullptr);

    hid_t d1 = H5Dcreate(file_id, "/edges/id", H5T_NATIVE_UINT32, space,
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    hid_t d2 = H5Dcreate(file_id, "/edges/from", H5T_NATIVE_UINT32, space,
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    hid_t d3 = H5Dcreate(file_id, "/edges/to", H5T_NATIVE_UINT32, space,
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    hid_t d4 = H5Dcreate(file_id, "/edges/reactance", H5T_NATIVE_DOUBLE, space,
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    hid_t d5 = H5Dcreate(file_id, "/edges/max_capacity", H5T_NATIVE_DOUBLE, space,
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    H5Dwrite(d1, H5T_NATIVE_UINT32, H5S_ALL, H5S_ALL, H5P_DEFAULT, ids.data());
    H5Dwrite(d2, H5T_NATIVE_UINT32, H5S_ALL, H5S_ALL, H5P_DEFAULT, from.data());
    H5Dwrite(d3, H5T_NATIVE_UINT32, H5S_ALL, H5S_ALL, H5P_DEFAULT, to.data());
    H5Dwrite(d4, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, reactance.data());
    H5Dwrite(d5, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, capacity.data());

    H5Dclose(d1);
    H5Dclose(d2);
    H5Dclose(d3);
    H5Dclose(d4);
    H5Dclose(d5);
    H5Sclose(space);
}

// ============================================================
// Time Series
// ============================================================

void GridDatasetExporter::write_timeseries_hdf5(hid_t file_id) const {
    const auto& ts = *m_timeseries;

    const size_t N = ts.num_nodes();
    const size_t T = ts.num_timesteps();

    std::vector<double> demand_flat;
    std::vector<double> gen_flat;

    demand_flat.reserve(N * T);
    gen_flat.reserve(N * T);

    for (NodeID i = 0; i < N; ++i) {
        const auto& d = ts.demand(i);
        const auto& g = ts.generation(i);

        demand_flat.insert(demand_flat.end(), d.begin(), d.end());
        gen_flat.insert(gen_flat.end(), g.begin(), g.end());
    }

    hsize_t dims[2] = { static_cast<hsize_t>(N), static_cast<hsize_t>(T) };

    hid_t space = H5Screate_simple(2, dims, nullptr);

    hid_t d_demand = H5Dcreate(file_id, "/timeseries/demand",
                               H5T_NATIVE_DOUBLE, space,
                               H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    hid_t d_gen = H5Dcreate(file_id, "/timeseries/generation",
                            H5T_NATIVE_DOUBLE, space,
                            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    H5Dwrite(d_demand, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, demand_flat.data());
    H5Dwrite(d_gen, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, gen_flat.data());

    H5Dclose(d_demand);
    H5Dclose(d_gen);
    H5Sclose(space);
}