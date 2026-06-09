#include "GridExport.h"

#include <fstream>
#include <stdexcept>

namespace GridExport {

// -------------------------------------------------------------
// Export node attributes only (static)
// -------------------------------------------------------------
void export_static_nodes_csv(const InjectedGrid& ig, const std::string& filename)
{
    std::ofstream f(filename);
    if (!f.is_open())
        throw std::runtime_error("Failed to open node CSV: " + filename);

    const auto& grid = ig.grid();
    const auto& nodes = ig.nodes();

    if (nodes.size() != grid.num_nodes())
        throw std::runtime_error("Node attribute count mismatch");

    f << "node_id,type,max_capacity,base_load,operational,zone\n";
    for (NodeID id = 0; id < static_cast<NodeID>(nodes.size()); ++id) {
        const auto& n = nodes[id];
        f << id << ","
          << static_cast<int>(n.type) << ","
          << n.max_capacity << ","
          << n.base_load << ","
          << n.operational << ","
          << "\"" << n.zone << "\"\n";
    }
}

// -------------------------------------------------------------
// Export edge attributes along with topology (from/to)
// -------------------------------------------------------------
void export_static_edges_csv(const InjectedGrid& ig, const std::string& filename)
{
    std::ofstream f(filename);
    if (!f.is_open())
        throw std::runtime_error("Failed to open edge CSV: " + filename);

    const auto& grid = ig.grid();
    const auto& edges = ig.edges();

    if (edges.size() != grid.num_edges())
        throw std::runtime_error("Edge attribute count mismatch");

    f << "edge_id,from_node,to_node,max_capacity,reactance,operational,name\n";
    for (EdgeID id = 0; id < static_cast<EdgeID>(edges.size()); ++id) {
        const auto& topo = grid.edge(id);       // from/to nodes
        const auto& eattr = edges[id];          // injected attributes

        f << id << ","
          << topo.from << ","
          << topo.to << ","
          << eattr.max_capacity << ","
          << eattr.reactance << ","
          << eattr.operational << ","
          << "\"" << eattr.name << "\"\n";
    }
}

// -------------------------------------------------------------
// Full CSV export (node + edge, attributes + topology)
// -------------------------------------------------------------
void export_static_full_csv(const InjectedGrid& ig, const std::string& filename)
{
    std::ofstream f(filename);
    if (!f.is_open())
        throw std::runtime_error("Failed to open full CSV: " + filename);

    const auto& grid = ig.grid();
    const auto& nodes = ig.nodes();
    const auto& edges = ig.edges();

    if (nodes.size() != grid.num_nodes() || edges.size() != grid.num_edges())
        throw std::runtime_error("Topology/attribute mismatch");

    f << "type,element_id,attr,value\n";

    // ---------------- nodes ----------------
    for (NodeID id = 0; id < static_cast<NodeID>(nodes.size()); ++id) {
        const auto& n = nodes[id];
        f << "node," << id << ",type," << static_cast<int>(n.type) << "\n";
        f << "node," << id << ",max_capacity," << n.max_capacity << "\n";
        f << "node," << id << ",base_load," << n.base_load << "\n";
        f << "node," << id << ",operational," << n.operational << "\n";
        f << "node," << id << ",zone,\"" << n.zone << "\"\n";
    }

    // ---------------- edges ----------------
    for (EdgeID id = 0; id < static_cast<EdgeID>(edges.size()); ++id) {
        const auto& topo = grid.edge(id);       // from/to
        const auto& eattr = edges[id];          // attributes

        f << "edge," << id << ",from_node," << topo.from << "\n";
        f << "edge," << id << ",to_node," << topo.to << "\n";
        f << "edge," << id << ",max_capacity," << eattr.max_capacity << "\n";
        f << "edge," << id << ",reactance," << eattr.reactance << "\n";
        f << "edge," << id << ",operational," << eattr.operational << "\n";
        f << "edge," << id << ",name,\"" << eattr.name << "\"\n";
    }
}

} // namespace GridExport