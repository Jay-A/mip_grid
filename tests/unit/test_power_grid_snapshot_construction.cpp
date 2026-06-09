#include <iostream>
#include <stdexcept>

#include "test_utils.h"
#include "PowerGridSnapshot.h"

int main()
{
    // -------------------------------
    // Build grid & time series
    // -------------------------------
    InjectedGrid injected = make_test_injected_grid(10, 20);
    TimeSeries ts = make_test_timeseries(injected, 24);

    GridSpec spec{};

    // -------------------------------
    // Create TimeSeriesAttributes for the snapshot
    // -------------------------------
    TimeSeriesAttributes ts_attr;
    ts_attr.timesteps = ts.num_timesteps();   // match the generated timeseries
    ts_attr.sample_frequency_minutes = 60;    // arbitrary for test
    ts_attr.seed = 42;                        // deterministic

    // -------------------------------
    // Construct snapshot
    // -------------------------------
    PowerGridSnapshot snapshot(injected, ts, spec, ts_attr);

    // -------------------------------
    // Basic sanity checks
    // -------------------------------
    if (snapshot.num_nodes() != 10) return 1;
    if (snapshot.num_edges() != 20) return 1;
    if (snapshot.num_timesteps() != 24) return 1;

    return 0;
}

// #include <iostream>
// #include <stdexcept>

// #include "test_utils.h"
// #include "PowerGridSnapshot.h"

// int main()
// {
//     InjectedGrid injected = make_test_injected_grid(10, 20);
//     TimeSeries ts = make_test_timeseries(injected, 24);

//     GridSpec spec{};

//     PowerGridSnapshot snapshot(injected, ts, spec);

//     if (snapshot.num_nodes() != 10) return 1;
//     if (snapshot.num_edges() != 20) return 1;
//     if (snapshot.num_timesteps() != 24) return 1;

//     return 0;
// }