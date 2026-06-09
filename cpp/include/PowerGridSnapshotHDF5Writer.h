#pragma once

#include "PowerGridSnapshot.h"
#include "GridSpec.h"

#include <string>
#include <H5Cpp.h>

/**
 * @file PowerGridSnapshotHDF5Writer.h
 * @brief HDF5 serialization layer for PowerGridSnapshot.
 *
 * @defgroup hdf5_writer PowerGridSnapshotHDF5Writer
 * @brief Writes fully assembled power grid snapshots to HDF5 format.
 *
 * This writer converts a PowerGridSnapshot into a Python-friendly
 * HDF5 layout suitable for:
 * - Scientific analysis (NumPy / h5py)
 * - Solver integration
 * - Post-processing pipelines
 *
 * @section layout HDF5 Layout
 * - /meta
 *     Scalar metadata (GridSpec + dimensions)
 * - /nodes
 *     Node table: id, type, base_load, max_capacity
 * - /edges
 *     Edge table: id, from, to, reactance, max_capacity
 * - /time_series
 *     2D arrays:
 *       - demand     [num_nodes x num_timesteps]
 *       - generation [num_nodes x num_timesteps]
 *
 * @section design Design Philosophy
 * - Deterministic, lossless serialization of snapshot state
 * - Python-first layout (NumPy-compatible arrays)
 * - Strict separation of meta / topology / time series
 * - No modification of snapshot during export
 */
class PowerGridSnapshotHDF5Writer {
public:

    // ============================================================
    // Construction
    // ============================================================

    /**
     * @brief Construct writer bound to a snapshot.
     *
     * @param snapshot Fully constructed PowerGridSnapshot (read-only)
     */
    explicit PowerGridSnapshotHDF5Writer(const PowerGridSnapshot& snapshot);

    // ============================================================
    // Export API
    // ============================================================

    /**
     * @brief Write snapshot to HDF5 file.
     *
     * @param filename Output HDF5 file path
     *
     * @throws H5::Exception on HDF5 write failure
     * @throws std::runtime_error on logical inconsistencies
     */
    void write(const std::string& filename) const;

private:

    // ============================================================
    // Internal state
    // ============================================================

    const PowerGridSnapshot& m_snapshot_;

    // ============================================================
    // Internal writers
    // ============================================================

    void write_meta(H5::H5File& file) const;
    void write_nodes(H5::H5File& file) const;
    void write_edges(H5::H5File& file) const;
    void write_time_series(H5::H5File& file) const;
};