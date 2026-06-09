#include "TimeSeriesConstructor.h"

#include <cmath>
#include <random>
#include <stdexcept>
#include <algorithm>

/**
 * @brief Build node-wise demand and generation time series and wrap in immutable object.
 *
 * Each node gets a vector of timesteps. Load nodes use demand modulation,
 * generator nodes use generation modulation, bus nodes remain zero. 
 * Deterministic noise is added using per-node seeded RNG.
 */
TimeSeries TimeSeriesConstructor::construct(const InjectedGrid& grid,
                                            const TimeSeriesAttributes& attr) const 
{
    const std::size_t N = grid.nodes().size();
    const TimeStep T = static_cast<TimeStep>(attr.timesteps);

    // Temporary ephemeral node-wise storage
    std::vector<std::vector<double>> demand_series(N);
    std::vector<std::vector<double>> generation_series(N);

    for (NodeID nid = 0; nid < N; ++nid) {
        const auto& node = grid.nodes()[nid];

        demand_series[nid].resize(T, 0.0);
        generation_series[nid].resize(T, 0.0);

        // Deterministic seed per node
        std::uint64_t seed = make_seed(attr.seed, nid, node.type == NodeType::Generator);
        std::mt19937 rng(seed);
        std::uniform_real_distribution<double> noise_dist(-attr.noise_amplitude,
                                                          attr.noise_amplitude);

        switch (node.type) {
            case NodeType::Load:
                for (TimeStep t = 0; t < T; ++t) {
                    double mod = demand_modulation(t, T, attr.demand_modulation_amplitude);
                    demand_series[nid][t] = std::max(0.0, node.base_load * mod + noise_dist(rng));
                }
                break;

            case NodeType::Generator:
                for (TimeStep t = 0; t < T; ++t) {
                    double mod = generation_modulation(t, T, attr.generator_modulation_amplitude);
                    generation_series[nid][t] = std::max(0.0, node.max_capacity * mod + noise_dist(rng));
                }
                break;

            case NodeType::Bus:
            case NodeType::None:
            default:
                // already zero
                break;
        }
    }

    // Immutable transfer: move temporary vectors into TimeSeries
    return TimeSeries(std::move(grid),
                      std::move(demand_series),
                      std::move(generation_series));
}

// ============================================================
// Internal deterministic helpers
// ============================================================

double TimeSeriesConstructor::demand_modulation(TimeStep t, TimeStep T,
                                                double amplitude) const 
{
    if (amplitude == 0.0) return 1.0;
    double theta = 2.0 * M_PI * static_cast<double>(t) / static_cast<double>(T);
    return 1.0 + amplitude * std::sin(theta);
}

double TimeSeriesConstructor::generation_modulation(TimeStep t, TimeStep T,
                                                    double amplitude) const 
{
    if (amplitude == 0.0) return 1.0;
    double theta = 2.0 * M_PI * static_cast<double>(t) / static_cast<double>(T);
    return 1.0 + amplitude * std::sin(theta);
}

std::uint64_t TimeSeriesConstructor::make_seed(std::uint64_t seed,
                                               NodeID node_id,
                                               bool is_generator) const 
{
    // Deterministic per-node seed: simple XOR + shift combination
    return seed ^ (static_cast<std::uint64_t>(node_id) << 32) ^
           (is_generator ? 0xA5A5A5A5 : 0x5A5A5A5A);
}