#pragma once

/**
 * @file Edge.h
 * @brief Defines the Edge data structure representing a directed connection in the grid graph.
 *
 * This structure models a transmission line (edge) between two nodes in the grid,
 * including capacity constraints used in both simulation and MIP formulations.
 *
 * Type Dependencies (from Types.h):
 *   NodeID   -> std::uint32_t : Node identifier
 *   Capacity  -> double       : Edge capacity constraint
 *
 * Conceptual Role:
 *   - Represents directed connectivity in the grid topology
 *   - Used in grid generation, validation, and optimization models
 */

#include "Types.h"

/**
 * @struct Edge
 * @brief Directed edge connecting two nodes with a capacity constraint.
 */
struct Edge {
    /** @brief Source node identifier. */
    NodeID from;

    /** @brief Target node identifier. */
    NodeID to;

    /** @brief Maximum transmissible capacity of the edge. */
    Capacity capacity;
};
