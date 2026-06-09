#include "GridInjector.h"

#include <stdexcept>
#include <queue>
#include <vector>
#include <random>


GridInjector::GridInjector(uint64_t seed)
    : m_rng(seed)
{
}

// ============================================================
// RNG helpers
// ============================================================

static std::mt19937_64 make_rng(uint64_t seed) {
    return std::mt19937_64(seed);
}

static double uniform(double min, double max, std::mt19937_64& rng) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(rng);
}

static bool bernoulli(double p_fail, std::mt19937_64& rng) {
    std::bernoulli_distribution dist(p_fail);
    return !dist(rng); // true = operational
}

// ============================================================
// Node attributes builder
// ============================================================

std::vector<NodeAttributes> GridInjector::build_node_attributes(
    const Grid& grid, const GridSpec& spec)
{
    const size_t N = grid.num_nodes();
    std::vector<NodeAttributes> attrs;
    attrs.reserve(N);

    for (NodeID id = 0; id < N; ++id) {
        NodeType type = grid.node_type(id); // ID-based query
        NodeAttributes na(type);

        switch (type) {
            case NodeType::Generator:
                na.max_capacity = sample_generator_capacity(spec);
                na.base_load = 0.0;
                break;

            case NodeType::Load:
                na.max_capacity = 0.0;
                na.base_load = sample_load_demand(spec);
                break;

            case NodeType::Bus:
            default:
                na.max_capacity = 0.0;
                na.base_load = 0.0;
                break;
        }

        na.operational = sample_operational(spec.node_failure_prob);
        attrs.push_back(std::move(na));
    }

    return attrs;
}

// ============================================================
// Edge attributes builder
// ============================================================

std::vector<EdgeAttributes> GridInjector::build_edge_attributes(
    const Grid& grid, const GridSpec& spec)
{
    const size_t M = grid.num_edges();
    std::vector<EdgeAttributes> attrs;
    attrs.reserve(M);

    auto rng = make_rng(spec.seed + 1);

    for (EdgeID id = 0; id < M; ++id) {
        const Edge& e = grid.edge(id); // ID-based access

        EdgeAttributes ea;
        ea.max_capacity = uniform(spec.edge_capacity_min,
                                  spec.edge_capacity_max, rng);
        ea.reactance    = uniform(spec.edge_reactance_min,
                                  spec.edge_reactance_max, rng);
        ea.operational  = bernoulli(spec.edge_failure_prob, rng);

        attrs.push_back(std::move(ea));
    }

    return attrs;
}

// ============================================================
// Main injection
// ============================================================

InjectedGrid GridInjector::inject(const Grid& grid, const GridSpec& spec)
{
    auto nodes = build_node_attributes(grid, spec);
    auto edges = build_edge_attributes(grid, spec);

    return InjectedGrid(grid, std::move(nodes), std::move(edges));
}

// ============================================================
// Topology helpers (optional production-ready)
// ============================================================

double GridInjector::node_degree_weight(NodeID id, const Grid& grid) const
{
    if (id >= grid.num_nodes())
        throw std::out_of_range("GridInjector::node_degree_weight invalid NodeID");

    const auto& neighbors = grid.adjacency()[id];
    return static_cast<double>(neighbors.size());
}

bool GridInjector::is_cross_link(NodeID from, NodeID to, const Grid& grid) const
{
    if (from >= grid.num_nodes() || to >= grid.num_nodes())
        throw std::out_of_range("GridInjector::is_cross_link invalid NodeID");

    // Example heuristic: cross-links go beyond nearest neighbors
    const auto& neighbors = grid.adjacency()[from];
    for (NodeID nb : neighbors) {
        if (nb == to)
            return false;
    }
    return true;
}

// ============================================================
// Sampling helpers (deterministic)
// ============================================================

double GridInjector::uniform01()
{
    return uniform(0.0, 1.0, m_rng);
}

double GridInjector::sample_generator_capacity(const GridSpec& spec)
{
    return uniform(spec.generator_capacity_min,
                   spec.generator_capacity_max, m_rng);
}

double GridInjector::sample_load_demand(const GridSpec& spec)
{
    return uniform(spec.load_demand_min, spec.load_demand_max, m_rng);
}

double GridInjector::sample_line_capacity(const GridSpec& spec)
{
    return uniform(spec.edge_capacity_min, spec.edge_capacity_max, m_rng);
}

double GridInjector::sample_reactance(const GridSpec& spec)
{
    return uniform(spec.edge_reactance_min, spec.edge_reactance_max, m_rng);
}

bool GridInjector::sample_operational(double failure_rate)
{
    return bernoulli(failure_rate, m_rng);
}