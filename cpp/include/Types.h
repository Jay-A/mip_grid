#pragma once

/**
 * @file types.h
 * @brief Defines core type aliases and enumerations for the grid model.
 *
 * Centralized type definitions for identifiers, quantities, and node classifications
 * used throughout the MIP grid engine. All types map to standard primitives as listed:
 *
 * Type Aliases:
 *   NodeID    -> std::uint32_t   : Unique identifier for a node in the grid graph
 *   EdgeID    -> std::uint32_t   : Unique identifier for an edge in the grid graph
 *   TimeStep  -> std::uint32_t   : Discrete time index (future MIP horizon support)
 *   Power     -> double          : Power quantity (MW or normalized unit)
 *   Demand    -> double          : Electrical demand value
 *   Capacity  -> double          : Capacity constraint value
 *   BinaryVar -> std::uint8_t    : Binary decision variable (0/1)
 *
 * Enumerations:
 *   NodeType  : Functional classification of grid nodes
 *       - None      : Unassigned / invalid / placeholder state
 *       - Generator : Power injection node
 *       - Bus       : Transit node
 *       - Load      : Consumption node
 */

#include <cstdint>

/** @brief Unique identifier for a node in the grid graph. */
using NodeID = std::uint32_t;

/** @brief Unique identifier for an edge in the grid graph. */
using EdgeID = std::uint32_t;

/** @brief Discrete time index (future MIP horizon support). */
using TimeStep = std::uint32_t;

/** @brief Power quantity (MW or normalized unit). */
using Power = double;

/** @brief Electrical demand value. */
using Demand = double;

/** @brief Capacity constraint value. */
using Capacity = double;

/** @brief Binary decision variable (0/1). */
using BinaryVar = std::uint8_t;

/**
 * @enum NodeType
 * @brief Functional classification of grid nodes.
 *
 * Used for role assignment in GridGenerator and validation in GridValidator.
 */
enum class NodeType {
    None = 0,     ///< Unassigned / invalid / placeholder state
    Generator,    ///< Power injection node
    Bus,          ///< Transit node
    Load          ///< Consumption node
};
