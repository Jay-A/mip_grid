#pragma once

#include "Types.h"

#include <cstdint>
#include <vector>
#include <string>

/**
 * @file GridAttributes.h
 * @brief Defines node and edge attributes for synthetic power grids.
 *
 * @defgroup grid_attributes Grid Attributes
 * @brief Encapsulates physical and operational properties of nodes and edges.
 *
 * These structures provide a clean, immutable data layer for the
 * InjectedGrid and downstream workflows.
 *
 * @section type_summary Type Dependencies (from Types.h)
 *   - NodeType -> enum class : Node classification
 *   - Capacity -> double     : Maximum node/edge capacity
 *   - Demand   -> double     : Base load demand
 */

/**
 * @struct NodeAttributes
 * @ingroup grid_attributes
 * @brief Physical and operational properties of a grid node.
 *
 * Stores generator/load capacities, base load, operational status,
 * and optional metadata such as zone.
 *
 * Examples:
 * - Generator capacity (MW)
 * - Load demand (MW)
 * - Operational status (on/off)
 */
struct NodeAttributes {
    NodeType type;          ///< Node functional type (Generator/Bus/Load)

    // Static properties
    Capacity max_capacity = 0.0; ///< Maximum generation or transfer capacity (MW)
    Demand base_load = 0.0;      ///< Base demand (MW)
    bool operational = true;     ///< Node online status

    // Optional metadata
    std::string zone;            ///< Geographic or logical zone

    /** @brief Construct NodeAttributes with optional NodeType (default Bus). */
    NodeAttributes(NodeType t = NodeType::Bus) : type(t) {}
};

/**
 * @struct EdgeAttributes
 * @ingroup grid_attributes
 * @brief Physical and operational properties of a grid edge.
 *
 * Stores line capacity, reactance, operational status, and optional metadata.
 *
 * Examples:
 * - Line capacity (MW)
 * - Impedance/reactance (pu)
 * - Operational status (on/off)
 */
struct EdgeAttributes {
    Capacity max_capacity = 0.0; ///< Maximum power flow (MW)
    double reactance = 0.0;      ///< Per-unit reactance (pu)
    bool operational = true;     ///< Edge online status

    // Optional metadata
    std::string name;            ///< Edge label or identifier

    /** @brief Default constructor for EdgeAttributes. */
    EdgeAttributes() = default;
};