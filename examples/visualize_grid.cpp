#include "GridGenerator.h"
#include "DotGraphExporter.h"

#include <iostream>

int main()
{
    // ------------------------------------------------------------
    // 1. Define a small spec for visualization
    // ------------------------------------------------------------
    GridSpec spec = GridSpec::Minimal(20);

    // Optional: tweak for clearer visuals
    spec.use_ring_backbone = true;   // easier to see cycles
    spec.seed = 42;                  // deterministic output

    // ------------------------------------------------------------
    // 2. Generate grid
    // ------------------------------------------------------------
    GridGenerator gen;
    Grid grid = gen.generate(spec);

    // ------------------------------------------------------------
    // 3. Export to DOT
    // ------------------------------------------------------------
    const std::string filename = "grid.dot";

    DotGraphExporter::export_dot(grid, filename);

    std::cout << "Graph exported to: " << filename << "\n";
    std::cout << "Render with:\n";
    std::cout << "  dot -Tpng grid.dot -o grid.png\n";

    return 0;
}
