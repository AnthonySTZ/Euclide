#pragma once

#include "node.h"

namespace butter {

class Scene {
    void addNode(const std::shared_ptr<Node>& t_node);

private:
    std::vector<std::shared_ptr<Node>> m_nodes;

};

}