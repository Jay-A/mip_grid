#pragma once

#include <cstddef>
#include <cstdint>

#include "Grid.h"
#include "GridSpec.h"

/**
 * @file GridValidator.h
 * @brief Rule-based validation engine for synthetic power grid graphs.
 *
 * This file defines GridValidator, a reusable evaluation object that
 * checks whether a generated Grid satisfies a given set of validation rules.
 *
 * GridValidator is intentionally decoupled from GridGenerator and GridSpec.
 * It operates only on fully constructed Grid objects and does not participate
 * in graph construction.
 *
 * This design enables:
 * - Independent correctness verification
 * - Batch validation of large-scale graphs (10k+ nodes)
 * - Reusable rule contexts across multiple grids
 * - MPI-friendly stateless evaluation workflows (aside from rule configuration)
 */

/**
 * @struct ValidationRules
 * @brief Configuration object defining feasibility and structural constraints.
 *
 * ValidationRules defines how a Grid should be evaluated.
 * It does not contain graph data or state.
 */
struct ValidationRules {

    /**
     * @brief Require the graph to be fully connected.
     *
     * If true, all nodes must belong to a single connected component.
     */
    bool require_connected = true;

    /**
     * @brief Maximum allowed fraction of loads not connected to any generator.
     *
     * Value range:
     * - 0.0 b
     * - 1.0 b
     */
    double max_unserved_load_fraction = 0.0;
};

/**
 * @class GridValidator
 * @brief Evaluation engine for validating Grid objects against rules.
 *
 * GridValidator is a lightweight, reusable object that applies a fixed
 * set of ValidationRules to one or more Grid instances.
 *
 * The validator may precompute internal structures derived from rules
 * to improve performance in batch validation scenarios.
 */
class GridValidator {
public:

    /**
     * @brief Construct a validator with a given rule set.
     *
     * @param rules ValidationRules defining feasibility constraints.
     */
    explicit GridValidator(const ValidationRules& rules);

    /**
     * @brief Check whether a grid satisfies the configured validation rules.
     *
     * @param grid The Grid object to validate.
     * @return true if the grid satisfies all rules, false otherwise.
     */
    bool is_valid(const Grid& grid) const;

    /**
     * @brief Check whether the grid is fully connected.
     *
     * @param grid The Grid object to evaluate.
     * @return true if all nodes are in a single connected component.
     */
    bool is_connected(const Grid& grid) const;

    /**
     * @brief Compute fraction of loads not reachable from any generator.
     *
     * @param grid The Grid object to evaluate.
     * @return Fraction in range [0.0, 1.0].
     */
    double unserved_load_fraction(const Grid& grid) const;

private:

    /**
     * @brief Stored validation rules for this validator instance.
     */
    ValidationRules rules_;
};
