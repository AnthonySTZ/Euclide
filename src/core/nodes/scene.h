#pragma once

#include "node.h"

#include <unordered_map>
#include <string>

namespace butter {

class Scene {

public:
    size_t size() const noexcept { return m_nodes.size(); }
    void addNode(const std::shared_ptr<Node>& t_node);
    std::shared_ptr<Node> node(const std::string& t_name) const noexcept;
    
private:
    std::string findName(const std::string &t_name) const;
    std::unordered_map<std::string, std::shared_ptr<Node>> m_nodes;

};

}