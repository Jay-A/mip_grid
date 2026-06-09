#pragma once

#include "PowerGridSnapshot.h"  // renamed from GridDatasetBundle
#include "GridSpec.h"

#include <string>
#include <H5Cpp.h>

/**
 * @brief Writes a PowerGridSnapshot to HDF5 in a Python-friendly layout.
 *
 * Layout:
 *   /meta         : scalar attributes from GridSpec + num_nodes, num_edges, num_timesteps
 *   /nodes        : table of node attributes (id, type, base_load, max_capacity)
 *   /edges        : table of edge attributes (id, from, to, reactance, max_capacity)
 *   /time_series  : 2D arrays
 *                    /time_series/demand       shape [num_nodes x num_timesteps]
 *                    /time_series/generation   shape [num_nodes x num_timesteps]
 */
class PowerGridSnapshotHDF5Writer {
public:
    explicit PowerGridSnapshotHDF5Writer(const PowerGridSnapshot& snapshot);

    void write(const std::string& filename) const;

private:
    const PowerGridSnapshot& m_snapshot_;

    void write_meta(H5::H5File& file) const;
    void write_nodes(H5::H5File& file) const;
    void write_edges(H5::H5File& file) const;
    void write_time_series(H5::H5File& file) const;
};
