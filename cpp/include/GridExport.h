// GridExport.h
#pragma once

#include <string>
#include "InjectedGrid.h"

namespace GridExport {
    void export_static_nodes_csv(const InjectedGrid& ig, const std::string& filename);
    void export_static_edges_csv(const InjectedGrid& ig, const std::string& filename);
    void export_static_full_csv(const InjectedGrid& ig, const std::string& filename);
}