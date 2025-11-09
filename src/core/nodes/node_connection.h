#pragma once

#include "node.h"

#include <memory>

namespace butter {

class Node;

struct NodeConnection{

    NodeConnection() = default;
    ~NodeConnection() = default;

    size_t sourceIndex;
    size_t destIndex;
    std::weak_ptr<Node> sourceNode;
    std::weak_ptr<Node> destNode;

    NodeConnection(const size_t t_sourceIndex, const size_t t_destIndex,
        std::shared_ptr<Node> t_sourceNode,
        std::shared_ptr<Node> t_destNode)
        : sourceIndex(t_sourceIndex), destIndex(t_destIndex), sourceNode(t_sourceNode), destNode(t_destNode) {}

};

}