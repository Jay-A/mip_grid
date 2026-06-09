#pragma once

#include <string>

class Grid;

class DotGraphExporter {
public:
    static void export_dot(
        const Grid& grid,
        const std::string& filename);
};
