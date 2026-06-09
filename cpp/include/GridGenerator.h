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
 * @defgroup grid_generator GridGenerator
 * @brief Deterministic synthetic grid construction from declarative specifications.
 *
 * GridGenerator constructs deterministic synthetic grid topologies
 * from a GridSpec. It does not store generated grids and retains no state
 * beyond optional RNG configuration.
 *
 * @section design Design philosophy
 * - Stateless: no persistent grid data
 * - Deterministic output for identical (spec, seed)
 * - Lightweight preflight spec validation
 * - Correctness validated externally via GridValidator
 *
 * @section use_cases Targeted usage
 * - Large-scale synthetic grid creation (10k+ nodes)
 * - MPI-parallel execution (independent instances per rank)
 * - Reproducible benchmarking and solver testing
 *
 * @section type_summary Type dependencies
 * - GridSpec : declarative grid specification
 * - Grid     : generated immutable grid container
 */
class GridGenerator {
public:

    // ============================================================
    // Construction
    // ============================================================

    /**
     * @brief Construct generator with optional deterministic seed.
     *
     * @param seed Deterministic seed for reproducible generation (uint64_t)
     */
    explicit GridGenerator(uint64_t seed = 0);

    // ============================================================
    // Preflight / validation
    // ============================================================

    /**
     * @brief Lightweight validation of a GridSpec before generation.
     *
     * Determines whether a GridSpec is sufficient and safe
     * for synthetic grid construction.
     *
     * @details
     * - Does NOT modify the specification
     * - Does NOT inspect any Grid state
     * - Only validates feasibility
     *
     * @param spec Input grid specification (GridSpec)
     * @param error Optional output string for descriptive error message
     * @return true if generation should proceed, false otherwise
     */
    bool spec_check(const GridSpec& spec,
                    std::string& error) const;

    // ============================================================
    // Grid generation
    // ============================================================

    /**
     * @brief Generate a synthetic grid from a declarative specification.
     *
     * @param spec GridSpec defining nodes, edges, and parameters
     * @return Grid Fully constructed immutable Grid object
     *
     * @details Guarantees:
     * - Deterministic output for identical (spec, seed)
     * - Input specification is not modified
     * - No internal grid state is retained
     */
    Grid generate(const GridSpec& spec);

private:

    // ============================================================
    // Internal state
    // ============================================================

    /**
     * @brief Internal random engine used for deterministic sampling.
     *
     * This engine is NOT part of the grid state and is only used
     * during construction of Grid objects.
     */
    std::mt19937 m_random_engine;
};