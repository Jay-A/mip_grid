#pragma once

#include "Types.h"

#include <cstdint>
#include <vector>
#include <string>

/**
 * @file GridAttributes.h
 * @brief Defines the attributes for nodes and edges in a power grid.
 *
 * This is used by InjectedGrid as a clean, immutable data layer.
 */

/**
 * @struct NodeAttributes
 * @brief Stores the physical and operational properties of a node.
 *
 * Examples:
 * - Generator capacity (MW)
 * - Load demand (MW)
 * - Operational status (on/off)
 */
struct NodeAttributes {
    NodeType type;

    // Static properties
    Capacity max_capacity = 0.0;
    Demand base_load = 0.0;
    // double max_capacity = 0.0;     // MW, for generators
    // double base_load    = 0.0;     // MW, for loads
    bool operational    = true;    // is the node online?

    // Optional metadata
    std::string zone;              // geographic or logical zone

    // Constructor
    NodeAttributes(NodeType t = NodeType::Bus) : type(t) {}
};

/**
 * @struct EdgeAttributes
 * @brief Stores the physical and operational properties of an edge.
 *
 * Examples:
 * - Line capacity (MW)
 * - Impedance (pu)
 * - Operational status (on/off)
 */
struct EdgeAttributes {
    double max_capacity = 0.0;     // MW
    double reactance    = 0.0;     // pu
    bool operational    = true;    // is the line online?

    // Optional metadata
    std::string name;

    // Constructor
    EdgeAttributes() = default;
};
