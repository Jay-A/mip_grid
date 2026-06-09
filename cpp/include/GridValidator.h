#pragma once

#include <cstddef>
#include <cstdint>

#include "Grid.h"
#include "GridSpec.h"

/**
 * @file GridValidator.h
 * @brief Rule-based validation engine for synthetic power grid graphs.
 *
 * @defgroup grid_validator GridValidator
 * @brief Evaluates whether a constructed Grid satisfies structural and feasibility rules.
 *
 * GridValidator operates on fully constructed Grid objects.
 * It does NOT participate in grid construction and is fully decoupled
 * from GridGenerator, GridSpec, and GridInjector.
 *
 * ## Design Goals
 * - Independent correctness verification
 * - Reusable rule contexts for batch validation
 * - Lightweight, MPI-friendly evaluation (stateless aside from rules)
 * - Efficient precomputation of derived rule structures
 */

/**
 * @struct ValidationRules
 * @ingroup grid_validator
 * @brief Feasibility and structural constraints for grid validation.
 *
 * This object defines how a Grid should be evaluated.
 * It contains no runtime graph state.
 */
struct ValidationRules {

    // ============================================================
    // Connectivity rules
    // ============================================================

    bool require_connected = true; ///< Require a single connected component

    // ============================================================
    // Load service rules
    // ============================================================

    double max_unserved_load_fraction = 0.0; ///< Maximum fraction of loads not reachable from a generator
};

/**
 * @class GridValidator
 * @ingroup grid_validator
 * @brief Evaluation engine for validating Grid objects against rules.
 *
 * GridValidator applies a fixed ValidationRules instance to one or more Grids.
 * It may precompute internal structures derived from the rules for efficiency.
 */
class GridValidator {
public:

    // ============================================================
    // Construction
    // ============================================================

    /**
     * @brief Construct a validator with a given rule set.
     *
     * @param rules ValidationRules defining feasibility constraints.
     */
    explicit GridValidator(const ValidationRules& rules);

    // ============================================================
    // Validation API
    // ============================================================

    /**
     * @brief Check whether a grid satisfies all configured validation rules.
     *
     * @param grid Grid to evaluate
     * @return true if all rules are satisfied
     */
    bool is_valid(const Grid& grid) const;

    /**
     * @brief Check if all nodes are in a single connected component.
     *
     * @param grid Grid to evaluate
     * @return true if fully connected
     */
    bool is_connected(const Grid& grid) const;

    /**
     * @brief Compute fraction of loads not reachable from any generator.
     *
     * @param grid Grid to evaluate
     * @return Fraction in range [0.0, 1.0]
     */
    double unserved_load_fraction(const Grid& grid) const;

private:

    // ============================================================
    // Internal state
    // ============================================================

    ValidationRules rules_; ///< Stored validation rules
};