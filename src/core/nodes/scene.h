#pragma once

#include "node.h"

#include <unordered_map>
#include <string>

namespace butter {

class Scene {

public:
    void addNode(const std::shared_ptr<Node>& t_node);

    
private:
    std::string findName(const std::string &t_name);
    std::unordered_map<std::string, std::shared_ptr<Node>> m_nodes;

};

}