#pragma once

#include <string>
#include "InjectedGrid.h"
#include "TimeSeries.h"
#include "hdf5.h"

/**
 * @file GridDatasetExporter.h
 * @brief Export a grid and its associated time series to disk (HDF5).
 *
 * Supports future extension to multiple formats (CSV, Parquet, etc.)
 * but currently implements HDF5-only export.
 */
class GridDatasetExporter {
public:

    /**
     * @brief Construct an exporter for a given grid and optional time series.
     *
     * @param grid Injected grid (node & edge attributes)
     * @param ts Optional time series corresponding to the grid
     */
    explicit GridDatasetExporter(const InjectedGrid& grid,
                                 const TimeSeries* ts = nullptr);

    /**
     * @brief Export the grid and time series to an HDF5 file.
     *
     * @param filename Path to output HDF5 file
     *
     * @throws std::runtime_error on failure
     */
    void export_hdf5(const std::string& filename) const;

private:
    const InjectedGrid& m_grid;
    const TimeSeries* m_timeseries;

    // Internal helpers for node, edge, and time series dataset writing
    void write_nodes_hdf5(hid_t file_id) const;
    void write_edges_hdf5(hid_t file_id) const;
    void write_timeseries_hdf5(hid_t file_id) const;
};
