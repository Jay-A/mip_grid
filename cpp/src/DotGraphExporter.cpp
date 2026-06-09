#include "DotGraphExporter.h"
#include "Grid.h"

#include <fstream>
#include <string>

void DotGraphExporter::export_dot(
    const Grid& grid,
    const std::string& filename)
{
    std::ofstream out(filename);
    if (!out.is_open()) return;

    out << "digraph Grid {\n";
    out << "    node [shape=circle];\n";

    // ---------- Node colors ----------
    auto color_for = [](NodeType t) -> const char* {
        switch (t) {
            case NodeType::Generator: return "red";
            case NodeType::Load:      return "blue";
            case NodeType::Bus:       return "gray";
            default:                  return "black";
        }
    };

    // Iterate over NodeIDs directly (no bulk accessor needed)
    for (NodeID i = 0; i < grid.num_nodes(); ++i) {
        NodeType t = grid.node_type(i);
        out << "    "
            << i
            << " [style=filled, fillcolor="
            << color_for(t)
            << "];\n";
    }

    // ---------- Edges ----------
    for (EdgeID i = 0; i < grid.num_edges(); ++i) {
        const Edge& e = grid.edge(i);
        out << "    "
            << e.from
            << " -> "
            << e.to
            << ";\n";
    }

    // ---------- Legend ----------
    out << "    subgraph cluster_legend {\n";
    out << "        label=\"Legend\";\n";
    out << "        fontsize=18;\n";
    out << "        color=black;\n";
    out << "        keyGen [label=\"Generator\", style=filled, fillcolor=red];\n";
    out << "        keyLoad [label=\"Load\", style=filled, fillcolor=blue];\n";
    out << "        keyBus [label=\"Bus\", style=filled, fillcolor=gray];\n";
    out << "    }\n";

    out << "}\n";
}

// #include "DotGraphExporter.h"
// #include "Grid.h"

// #include <fstream>

// void DotGraphExporter::export_dot(
//     const Grid& grid,
//     const std::string& filename)
// {
//     std::ofstream out(filename);
//     if (!out.is_open()) return;

//     out << "digraph Grid {\n";
//     out << "    node [shape=circle];\n";
    
//     const auto& types = grid.node_type();

//     // ---------- Node colors ----------
//     auto color_for = [](NodeType t) -> const char* {
//         switch (t) {
//             case NodeType::Generator: return "red";
//             case NodeType::Load:      return "blue";
//             case NodeType::Bus:       return "gray";
//             default:                  return "black";
//         }
//     };

//     for (NodeID i = 0; i < types.size(); ++i) {
//         out << "    "
//             << i
//             << " [style=filled, fillcolor="
//             << color_for(types[i])
//             << "];\n";
//     }

//     // ---------- Edges ----------
//     for (const auto& e : grid.edges()) {
//         out << "    "
//             << e.from
//             << " -> "
//             << e.to
//             << ";\n";
//     }

//     // ---------- Legend ----------
//     out << "    subgraph cluster_legend {\n";
//     out << "        label=\"Legend\";\n";
//     out << "        fontsize=18;\n";
//     out << "        color=black;\n";
//     out << "        keyGen [label=\"Generator\", style=filled, fillcolor=red];\n";
//     out << "        keyLoad [label=\"Load\", style=filled, fillcolor=blue];\n";
//     out << "        keyBus [label=\"Bus\", style=filled, fillcolor=gray];\n";
//     out << "    }\n";

//     out << "}\n";
// }