// tests/unit/test_grid_injected.cpp
#include "GridGenerator.h"
#include "GridInjector.h"
#include <cassert>
#include <iostream>

int main() {
    // Grid construction
    GridSpec spec = GridSpec::Minimal(10);
    GridGenerator gen;
    Grid grid = gen.generate(spec);
    assert(grid.num_nodes() == 10);
    assert(grid.num_edges() >= 0);

    // Injected attributes
    GridInjector injector(12345);
    InjectedGrid ig1 = injector.inject(grid, spec);
    InjectedGrid ig2 = injector.inject(grid, spec);

    assert(ig1.nodes().size() == ig2.nodes().size());
    assert(ig1.edges().size() == ig2.edges().size());
    assert(ig1.nodes()[0].max_capacity == ig2.nodes()[0].max_capacity);

    std::cout << "[OK] GridConstruction and InjectedAttributes deterministic.\n";
    return 0;
}