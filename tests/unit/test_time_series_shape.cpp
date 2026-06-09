#include <iostream>
#include <cassert>

#include "GridGenerator.h"
#include "GridInjector.h"
#include "TimeSeriesConstructor.h"
#include "TimeSeriesAttributes.h"

int main() {
    GridSpec spec = GridSpec::Minimal(10);

    GridGenerator gen;
    Grid grid = gen.generate(spec);

    GridInjector injector(123);
    InjectedGrid ig = injector.inject(grid, spec);

    TimeSeriesAttributes attr;
    attr.timesteps = 24;
    attr.seed = 42;

    TimeSeriesConstructor ctor;
    TimeSeries ts = ctor.construct(ig, attr);

    assert(ts.num_nodes() == ig.nodes().size());
    assert(ts.num_timesteps() == 24);

    for (NodeID i = 0; i < ts.num_nodes(); ++i) {
        assert(ts.demand(i).size() == 24);
        assert(ts.generation(i).size() == 24);
    }

    std::cout << "[OK] TimeSeries shape test passed\n";
    return 0;
}
