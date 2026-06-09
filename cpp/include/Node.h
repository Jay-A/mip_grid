#pragma once

#include "Types.h"
#include <cstdint>

/**
 * @file Node.h
 * @brief Lightweight node identifier wrapper.
 *
 * This struct provides a minimal encapsulation of a node ID.
 * It is primarily used as a convenience type for algorithms
 * and interfaces that require node references.
 *
 * @section type_summary Type dependencies
 * - NodeID -> std::uint32_t : Unique node identifier
 */
struct Node {
    /// Unique identifier for this node
    NodeID id;
};