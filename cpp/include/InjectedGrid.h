#pragma once

#include "GridAttributes.h"
#include "Types.h"
#include "Grid.h"

#include <vector>
#include <stdexcept>

/**
 * @file InjectedGrid.h
 * @brief Stores a grid along with its injected node and edge attributes.
 *
 * This is a read-only view after construction; all attributes are set
 * by GridInjector.
 */
class InjectedGrid {
public:
    // Construct fully-initialized InjectedGrid
    InjectedGrid(const Grid& g,
                 std::vector<NodeAttributes>&& node_attrs,
                 std::vector<EdgeAttributes>&& edge_attrs)
        : grid_(g),
          nodes_(std::move(node_attrs)),
          edges_(std::move(edge_attrs))
    {
        // Optional: sanity check
        if (nodes_.size() != grid_.num_nodes()) {
            throw std::runtime_error("Injected node attributes size mismatch");
        }
        if (edges_.size() != grid_.num_edges()) {
            throw std::runtime_error("Injected edge attributes size mismatch");
        }
    }

    // Disable default constructor
    InjectedGrid() = delete;

    // Copy and move constructors
    InjectedGrid(const InjectedGrid&) = default;
    InjectedGrid(InjectedGrid&&) = default;
    InjectedGrid& operator=(const InjectedGrid&) = default;
    InjectedGrid& operator=(InjectedGrid&&) = default;

    // Accessors
    const Grid& grid() const { return grid_; }
    const std::vector<NodeAttributes>& nodes() const { return nodes_; }
    const std::vector<EdgeAttributes>& edges() const { return edges_; }

    // Optional: per-node / per-edge accessor
    const NodeAttributes& node(NodeID id) const { return nodes_.at(id); }
    const EdgeAttributes& edge(EdgeID id) const { return edges_.at(id); }

private:
    const Grid& grid_;
    std::vector<NodeAttributes> nodes_;
    std::vector<EdgeAttributes> edges_;
};