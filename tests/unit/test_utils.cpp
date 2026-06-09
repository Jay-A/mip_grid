#include "test_utils.h"

// ============================================================
// Grid
// ============================================================

Grid make_test_grid(std::size_t nodes, std::size_t edges)
{
    GridBuilder builder;

    std::vector<NodeID> node_ids;
    node_ids.reserve(nodes);

    for (std::size_t i = 0; i < nodes; ++i) {
        node_ids.push_back(builder.add_node());
    }

    for (std::size_t e = 0; e < edges; ++e) {
        NodeID from = node_ids[e % nodes];
        NodeID to   = node_ids[(e + 1) % nodes];

        builder.add_edge(from, to);
    }

    return std::move(builder).build();
}

// ============================================================
// InjectedGrid
// ============================================================

InjectedGrid make_test_injected_grid(std::size_t nodes,
                                     std::size_t edges)
{
    Grid g = make_test_grid(nodes, edges);

    std::vector<NodeAttributes> node_attrs(nodes);
    std::vector<EdgeAttributes> edge_attrs(edges);

    for (auto& n : node_attrs) {
        n.type = NodeType::Load;
        n.base_load = 1.0;
        n.max_capacity = 10.0;
    }

    for (auto& e : edge_attrs) {
        e.reactance = 0.1;
        e.max_capacity = 100.0;
    }

    return InjectedGrid(g,
                         std::move(node_attrs),
                         std::move(edge_attrs));
}

// ============================================================
// TimeSeries
// ============================================================

TimeSeries make_test_timeseries(const InjectedGrid& grid,
                                std::size_t timesteps)
{
    const std::size_t N = grid.grid().num_nodes();

    std::vector<std::vector<double>> demand(
        N, std::vector<double>(timesteps, 1.0));

    std::vector<std::vector<double>> generation(
        N, std::vector<double>(timesteps, 0.0));

    return TimeSeries(grid,
                      std::move(demand),
                      std::move(generation));
}

// ============================================================
// Dataset
// ============================================================

TestDataset make_test_dataset(std::size_t nodes,
                              std::size_t edges,
                              std::size_t timesteps)
{
    InjectedGrid injected =
        make_test_injected_grid(nodes, edges);

    TimeSeries ts =
        make_test_timeseries(injected, timesteps);

    return TestDataset{
        make_test_grid(nodes, edges),
        std::move(injected),
        std::move(ts)
    };
}