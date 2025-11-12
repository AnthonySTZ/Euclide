#pragma once

#include "node.h"

#include <vector>
#include <functional>
#include <memory>


namespace butter {
    
struct NodeMenuItem {
    std::string name;
    std::function<std::shared_ptr<Node>()> createNode;
};

class NodesInfo {
    public:
    static const std::vector<std::pair<std::string, std::vector<NodeMenuItem>>>& getMenuItems();
};

}