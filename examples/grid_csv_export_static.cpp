#include "GridGenerator.h"
#include "GridInjector.h"
#include "GridExport.h"

#include <iostream>

int main()
{
    // ------------------------------------------------------------
    // 1. Define grid specification
    // ------------------------------------------------------------
    GridSpec spec = GridSpec::Minimal(20);

    // Determinism controls
    spec.seed = 42;
    spec.use_ring_backbone = true;

    // ------------------------------------------------------------
    // 2. Generate topology
    // ------------------------------------------------------------
    GridGenerator gen;
    Grid grid = gen.generate(spec);

    // ------------------------------------------------------------
    // 3. Inject static attributes
    // ------------------------------------------------------------
    GridInjector injector;
    InjectedGrid ig = injector.inject(grid, spec);

    // ------------------------------------------------------------
    // 4. Export static CSV
    // ------------------------------------------------------------
    const std::string node_file = "nodes.csv";
    const std::string edge_file = "edges.csv";
    const std::string full_file = "grid_static.csv";

    GridExport::export_static_nodes_csv(ig, node_file);
    GridExport::export_static_edges_csv(ig, edge_file);
    GridExport::export_static_full_csv(ig, full_file);

    // ------------------------------------------------------------
    // 5. Console output
    // ------------------------------------------------------------
    std::cout << "Static grid exported successfully:\n";
    std::cout << "  Nodes: " << node_file << "\n";
    std::cout << "  Edges: " << edge_file << "\n";
    std::cout << "  Full : " << full_file << "\n";

    return 0;
}
