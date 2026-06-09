#pragma once

#include <string>
#include "InjectedGrid.h"

/**
 * @file GridExport.h
 * @brief CSV export utilities for InjectedGrid data.
 *
 * @defgroup grid_export Grid Export Utilities
 * @brief Lightweight export functions for static grid data.
 *
 * This module provides CSV-based export functionality for node and edge
 * data contained in an InjectedGrid. It is intended for debugging,
 * inspection, and lightweight analysis workflows.
 *
 * @section formats Supported Formats
 * - CSV (nodes, edges, full combined export)
 *
 * @section dependencies Type Dependencies
 * - InjectedGrid : enriched grid containing node and edge attributes
 */

/**
 * @namespace GridExport
 * @brief Collection of CSV export utilities for InjectedGrid.
 *
 * Provides functions to export node data, edge data, or full grid snapshots.
 */
namespace GridExport {

    /**
     * @brief Export node attributes to CSV.
     * @param ig Injected grid source
     * @param filename Output CSV file path
     */
    void export_static_nodes_csv(const InjectedGrid& ig, const std::string& filename);

    /**
     * @brief Export edge attributes to CSV.
     * @param ig Injected grid source
     * @param filename Output CSV file path
     */
    void export_static_edges_csv(const InjectedGrid& ig, const std::string& filename);

    /**
     * @brief Export full grid (nodes + edges) to CSV.
     * @param ig Injected grid source
     * @param filename Output CSV file path
     */
    void export_static_full_csv(const InjectedGrid& ig, const std::string& filename);
}