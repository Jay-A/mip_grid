#include "JsonConfigLoader.h"
#include "GridGenerator.h"
#include "GridInjector.h"
#include "TimeSeriesConstructor.h"
#include "TimeSeries.h"
#include "InjectedGrid.h"
#include "Grid.h"
#include "PowerGridSnapshot.h"
#include "PowerGridSnapshotHDF5Writer.h"
#include "DotGraphExporter.h"

#include <iostream>
#include <string>
#include <stdexcept>

// ---------------------
// Command-line parsing
// ---------------------
struct CommandLineOptions
{
    std::string input_file;
    bool view_graph = false;
};

CommandLineOptions parse_args(int argc, char** argv)
{
    CommandLineOptions opts;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "-i" || arg == "--input")
        {
            if (i + 1 >= argc)
                throw std::runtime_error("--input requires a filename");

            opts.input_file = argv[++i];
        }
        else if (arg == "-v" || arg == "--view-graph")
        {
            opts.view_graph = true;
        }
        else if (arg == "-h" || arg == "--help")
        {
            std::cout
                << "Usage:\n"
                << "  powergrid -i config.json [-v]\n\n"
                << "Options:\n"
                << "  -i, --input <file>     JSON configuration\n"
                << "  -v, --view-graph       Export DOT graph\n"
                << "  -h, --help             Show help\n";
            std::exit(0);
        }
        else
        {
            throw std::runtime_error("Unknown argument: " + arg);
        }
    }

    if (opts.input_file.empty())
        throw std::runtime_error("Missing required argument: --input");

    return opts;
}

// ---------------------
// Main
// ---------------------
int main(int argc, char** argv)
{
    try
    {
        CommandLineOptions opts = parse_args(argc, argv);

        // Load configuration
        GridSpec spec;
        TimeSeriesAttributes ts_attr;
        JsonConfigLoader::load(opts.input_file, spec, ts_attr);

        // Generate grid topology
        GridGenerator generator;
        Grid grid = generator.generate(spec);

        // Visualization branch
        if (opts.view_graph)
        {
            const std::string filename = "grid.dot";
            DotGraphExporter::export_dot(grid, filename);
            std::cout
                << "Graph exported to " << filename << "\n"
                << "Render with:\n"
                << "  dot -Tpng " << filename << " -o grid.png\n";
            return 0;
        }

        // Normal pipeline: inject attributes, construct time series, snapshot, HDF5
        GridInjector injector;
        InjectedGrid injected = injector.inject(grid, spec);

        TimeSeriesConstructor ts_builder;
        TimeSeries ts = ts_builder.construct(injected, ts_attr);

        PowerGridSnapshot snapshot(injected, ts, spec, ts_attr);

        PowerGridSnapshotHDF5Writer writer(snapshot);
        writer.write("grid_timeseries.h5");

        std::cout << "Successfully wrote: grid_timeseries.h5\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

// #include "JsonConfigLoader.h"

// #include "GridGenerator.h"
// #include "GridInjector.h"

// #include "TimeSeriesConstructor.h"
// #include "TimeSeries.h"

// #include "InjectedGrid.h"
// #include "Grid.h"

// #include "PowerGridSnapshot.h"
// #include "PowerGridSnapshotHDF5Writer.h"

// #include <iostream>
// #include <string>

// void print_edges(const InjectedGrid& injected) {
//     const Grid& g = injected.grid();
//     const auto& edges_topology = g.edges();
//     const auto& edges_attr = injected.edges();

//     std::cout << "Edge data:\n";
//     for (size_t eid = 0; eid < edges_topology.size(); ++eid) {
//         const Edge& e = edges_topology[eid];
//         const EdgeAttributes& attr = edges_attr[eid];

//         std::cout << "Edge " << eid
//                   << ": from " << e.from
//                   << " to " << e.to
//                   << "\n";
//     }
// }

// int main(int argc, char** argv)
// {
//     if (argc != 2) {
//         std::cerr << "Usage: " << argv[0] << " <config.json>\n";
//         return 1;
//     }

//     const std::string config_file = argv[1];

//     try {
//         // ============================================================
//         // 1. Load config
//         // ============================================================
//         GridSpec spec;
//         TimeSeriesAttributes ts_attr;

//         JsonConfigLoader::load(config_file, spec, ts_attr);

//         // ============================================================
//         // 2. Generate grid topology
//         // ============================================================
//         GridGenerator generator;
//         Grid grid = generator.generate(spec);

//         // ============================================================
//         // 3. Inject physical attributes
//         // ============================================================
//         GridInjector injector;
//         InjectedGrid injected = injector.inject(grid, spec);

//         // ============================================================
//         // 4. Build time series
//         // ============================================================
//         TimeSeriesConstructor ts_builder;
//         TimeSeries ts = ts_builder.construct(injected, ts_attr);

//         // ============================================================
//         // 5. Build snapshot (final unified object)
//         // ============================================================
//         PowerGridSnapshot snapshot(
//             injected,
//             ts,
//             spec,
//             ts_attr
//         );

//         // ============================================================
//         // 6. Write HDF5
//         // ============================================================
//         PowerGridSnapshotHDF5Writer writer(snapshot);
//         writer.write("grid_timeseries.h5");

//         std::cout << "Successfully wrote: grid_timeseries.h5\n";
//     }
//     catch (const std::exception& e) {
//         std::cerr << "Fatal error: " << e.what() << "\n";
//         return 1;
//     }

//     return 0;
// }