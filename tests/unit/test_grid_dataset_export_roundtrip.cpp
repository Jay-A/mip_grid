#include "GridBuilder.h"
#include "GridInjector.h"
#include "TimeSeriesConstructor.h"
#include "TimeSeriesAttributes.h"
#include "GridDatasetExporter.h"

int main() {

    // ============================================================
    // 1. Build topology (NO attributes here)
    // ============================================================
    GridBuilder builder;

    NodeID n0 = builder.add_node();
    NodeID n1 = builder.add_node();

    builder.add_edge(n0, n1);

    Grid grid = std::move(builder).build();

    // ============================================================
    // 2. Inject attributes
    // ============================================================
    GridInjector injector(42);  // seed only

    GridSpec spec; // assume default or minimal config

    InjectedGrid injected = injector.inject(grid, spec);

    // ============================================================
    // 3. Build time series
    // ============================================================
    TimeSeriesAttributes ts_attr;
    ts_attr.timesteps = 24;
    ts_attr.seed = 123;

    TimeSeriesConstructor ts_builder;
    TimeSeries ts = ts_builder.construct(injected, ts_attr);

    // ============================================================
    // 4. Export
    // ============================================================
    GridDatasetExporter exporter(injected, &ts);
    exporter.export_hdf5("roundtrip_test.h5");

    return 0;
}
