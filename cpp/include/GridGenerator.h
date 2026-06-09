#pragma once

#include <cstdint>
#include <random>
#include <string>
#include "GridSpec.h"
#include "Grid.h"

/**
 * @file GridGenerator.h
 * @brief Stateless synthetic power grid generator.
 *
 * GridGenerator constructs deterministic synthetic grid topologies
 * from a declarative GridSpec. It does not store generated graphs
 * and does not retain state between calls (beyond RNG configuration).
 *
 * The generator is designed for:
 * - Large-scale synthetic grid creation (10k+ nodes)
 * - MPI-parallel execution (independent instances per rank)
 * - Reproducible benchmarking and solver testing
 *
 * ## Design philosophy
 * - No persistent grid state
 * - Deterministic output for identical (spec, seed)
 * - Preflight validation via GridGenerator::preflight_spec_check
 * - Full correctness validation handled by GridValidator
 */
class GridGenerator {
public:

    /**
     * @brief Construct generator with optional seed.
     *
     * @param seed Deterministic seed for reproducible generation
     */
    explicit GridGenerator(uint64_t seed = 0);

    /**
     * @brief quick check of GridSpec before generation.
     *
     * Determines whether a GridSpec is sufficient and safe
     * for grid construction.
     *
     * This function is intentionally lightweight and is used
     * internally by generate(), but may also be called externally.
     *
     * ## Behavior
     * - Does NOT modify the spec
     * - Does NOT inspect any Grid state
     * - Only validates generation feasibility
     *
     * @param spec Input grid specification
     * @param error Optional output error message if invalid
     * @return true if generation should proceed, false otherwise
     */
    bool spec_check(const GridSpec& spec,
                    std::string& error) const;

    /**
     * @brief Generate a synthetic grid from a specification.
     *
     * @param spec Declarative grid construction specification
     * @return Fully constructed Grid object
     *
     * ## Guarantees
     * - Output is deterministic for identical (spec, seed)
     * - Does not modify input specification
     * - Does not retain internal graph state
     */
    Grid generate(const GridSpec& spec);

private:

    /**
     * @brief Internal random engine used for deterministic sampling.
     *
     * This engine is NOT part of the grid state. It is only used
     * during construction of the Grid object.
     */
    std::mt19937 m_random_engine;
};
