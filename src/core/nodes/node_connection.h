#pragma once

#include "node.h"

#include <memory>

namespace butter {

struct NodeConnection{

    size_t topIndex;
    size_t bottomIndex;
    std::weak_ptr<Node> topNode;
    std::weak_ptr<Node> bottomNode;

    NodeConnection(size_t t_topIndex, size_t t_bottomIndex,
        std::shared_ptr<Node> t_topNode,
        std::shared_ptr<Node> t_bottomNode)
        : topIndex(t_topIndex), bottomIndex(t_bottomIndex), topNode(t_topNode), bottomNode(t_bottomNode) {}

};

}