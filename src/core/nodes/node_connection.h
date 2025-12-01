#pragma once

#include "node.h"
#include <memory>

namespace euclide {

/// @brief Forward declaration of Node to break circular dependency.
class Node;

/// @brief Represents a connection between two nodes.
struct NodeConnection {
    /// @brief Index of the output slot on the source node.
    size_t sourceIndex{0};

    /// @brief Index of the input slot on the destination node.
    size_t destIndex{0};

    /// @brief Weak pointer to the source node.
    std::weak_ptr<Node> sourceNode;

    /// @brief Weak pointer to the destination node.
    std::weak_ptr<Node> destNode;

    NodeConnection() = default;
    ~NodeConnection() = default;

    /// @brief Constructs a NodeConnection between two nodes and their respective input/output indices.
    /// @param t_sourceIndex Index of the output slot on the source node.
    /// @param t_destIndex Index of the input slot on the destination node.
    /// @param t_sourceNode Shared pointer to the source node.
    /// @param t_destNode Shared pointer to the destination node.
    NodeConnection(const size_t t_sourceIndex, const size_t t_destIndex, const std::shared_ptr<Node>& t_sourceNode,
                   const std::shared_ptr<Node>& t_destNode)
        : sourceIndex(t_sourceIndex), destIndex(t_destIndex), sourceNode(t_sourceNode), destNode(t_destNode) {}
};

} // namespace euclide
