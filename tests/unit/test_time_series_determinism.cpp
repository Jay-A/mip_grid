#include <iostream>
#include <cassert>

#include "GridGenerator.h"
#include "GridInjector.h"
#include "TimeSeriesConstructor.h"
#include "TimeSeriesAttributes.h"

int main() {

    GridSpec spec = GridSpec::Minimal(8);

    GridGenerator gen;
    Grid grid = gen.generate(spec);

    GridInjector injector(999);
    InjectedGrid ig = injector.inject(grid, spec);

    TimeSeriesAttributes attr;
    attr.timesteps = 12;
    attr.seed = 777;

    TimeSeriesConstructor ctor;

    TimeSeries a = ctor.construct(ig, attr);
    TimeSeries b = ctor.construct(ig, attr);

    for (NodeID i = 0; i < a.num_nodes(); ++i) {
        assert(a.demand(i) == b.demand(i));
        assert(a.generation(i) == b.generation(i));
    }

    std::cout << "[OK] Determinism test passed\n";
    return 0;
}
