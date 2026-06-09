#include "GridGenerator.h"
#include "GridSpec.h"

#include <iostream>

int main()
{
    // ------------------------------------------------------------
    // 1. Generate grid (new API)
    // ------------------------------------------------------------
    GridSpec spec = GridSpec::Minimal(2);

    GridGenerator gen;
    Grid grid = gen.generate(spec);

    // ------------------------------------------------------------
    // 2. Extract nodes from Grid (NOT generator)
    // ------------------------------------------------------------
    const auto& nodes = grid.nodes();

    // ------------------------------------------------------------
    // 3. Size check
    // ------------------------------------------------------------
    if (nodes.size() != 2) {
        std::cerr << "Expected 2 nodes, found "
                  << nodes.size()
                  << std::endl;
        return 1;
    }

    // ------------------------------------------------------------
    // 4. Deterministic ID check
    // ------------------------------------------------------------
    if (nodes[0].id != 0) {
        std::cerr << "Node 0 ID mismatch" << std::endl;
        return 1;
    }

    if (nodes[1].id != 1) {
        std::cerr << "Node 1 ID mismatch" << std::endl;
        return 1;
    }

    std::cout << "Node generation from GridSpec passed." << std::endl;
    return 0;
}
