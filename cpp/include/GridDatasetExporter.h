#pragma once

#include <string>
#include "InjectedGrid.h"
#include "TimeSeries.h"
#include "hdf5.h"

/**
 * @file GridDatasetExporter.h
 * @brief Exports a grid and its associated time series to HDF5 format.
 *
 * @defgroup dataset_export Grid Dataset Export
 * @brief Persistence layer for grid and time-series data.
 *
 * This module provides functionality to serialize an InjectedGrid
 * along with optional TimeSeries data into an HDF5 file.
 *
 * @section format Support
 * Currently supports:
 * - HDF5 (primary format)
 *
 * Future extensions may include CSV and Parquet exports.
 *
 * @section dependencies Type Dependencies
 * - InjectedGrid : structured grid with attributes
 * - TimeSeries   : temporal demand/generation data (optional)
 * - hid_t        : HDF5 file handle type
 */

/**
 * @class GridDatasetExporter
 * @brief Handles serialization of grid datasets to disk.
 */
class GridDatasetExporter {
public:

    /**
     * @brief Construct exporter for grid and optional time series.
     * @param grid Injected grid (node & edge attributes)
     * @param ts Optional time series (nullable)
     */
    explicit GridDatasetExporter(const InjectedGrid& grid,
                                 const TimeSeries* ts = nullptr);

    /**
     * @brief Export grid dataset to HDF5 file.
     * @param filename Output file path
     * @throws std::runtime_error on write failure
     */
    void export_hdf5(const std::string& filename) const;

private:
    const InjectedGrid& m_grid;
    const TimeSeries* m_timeseries;

    void write_nodes_hdf5(hid_t file_id) const;
    void write_edges_hdf5(hid_t file_id) const;
    void write_timeseries_hdf5(hid_t file_id) const;
};