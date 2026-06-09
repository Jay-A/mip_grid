#include <iostream>
#include <cassert>

#include "GridGenerator.h"
#include "GridInjector.h"
#include "TimeSeriesConstructor.h"
#include "TimeSeriesAttributes.h"

int main() {

    GridSpec spec = GridSpec::Minimal(6);

    GridGenerator gen;
    Grid grid = gen.generate(spec);

    GridInjector injector(123);
    InjectedGrid ig = injector.inject(grid, spec);

    TimeSeriesAttributes attr;
    attr.timesteps = 10;
    attr.seed = 1;

    TimeSeriesConstructor ctor;
    TimeSeries ts = ctor.construct(ig, attr);

    for (NodeID i = 0; i < ig.nodes().size(); ++i) {
        const auto& node = ig.nodes()[i];

        if (node.type == NodeType::Bus) {
            for (auto v : ts.demand(i)) assert(v == 0.0);
            for (auto v : ts.generation(i)) assert(v == 0.0);
        }

        if (node.type == NodeType::Load) {
            for (auto v : ts.demand(i)) assert(v >= 0.0);
        }

        if (node.type == NodeType::Generator) {
            for (auto v : ts.generation(i)) assert(v >= 0.0);
        }
    }

    std::cout << "[OK] Node alignment test passed\n";
    return 0;
}
