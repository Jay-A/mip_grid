#pragma once

/**
 * @file DotGraphExporter.h
 * @brief Exports a Grid object to a Graphviz DOT file.
 *
 * Provides functionality to serialize the grid structure into the DOT format,
 * suitable for visualization with Graphviz tools. Typically used for debugging
 * or analysis of grid topology.
 *
 * Public API:
 *   static void export_dot(const Grid& grid, const std::string& filename)
 *       - @param grid: The grid object to be exported
 *       - @param filename: Path to the output DOT file
 */

#include <string>

class Grid;

/**
 * @class DotGraphExporter
 * @brief Handles exporting of Grid instances to DOT format.
 */
class DotGraphExporter {
public:
    /**
     * @brief Serialize a grid to a DOT file.
     * @param grid The grid to export.
     * @param filename Path to the output DOT file.
     */
    static void export_dot(
        const Grid& grid,
        const std::string& filename);
};