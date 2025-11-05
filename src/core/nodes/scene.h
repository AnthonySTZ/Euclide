#pragma once

#include "node.h"

#include <unordered_map>
#include <string>

namespace butter {

class Scene {

public:
    Observer<uint32_t, std::shared_ptr<Node>> onNodeAdded;
    Observer<uint32_t> onNodeRemoved;

    size_t size() const noexcept { return m_nodes.size(); }
    void addNode(const std::shared_ptr<Node>& t_node);
    void removeNode(const std::string& t_name);
    std::shared_ptr<Node> node(const std::string& t_name) const noexcept;
    
private:
    std::string findName(const std::string &t_name) const;
    uint32_t findId() const;

    std::unordered_map<std::uint32_t, std::shared_ptr<Node>> m_nodes;
    std::unordered_map<std::string, std::uint32_t> m_nodesIds;

};

}