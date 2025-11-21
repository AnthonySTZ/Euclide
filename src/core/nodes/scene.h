#pragma once

#include "node.h"
#include "geometry/mesh.h"

#include <unordered_map>
#include <string>

namespace butter {

class Scene {

public:
    Observer<uint32_t, std::shared_ptr<Node>> onNodeAdded;
    Observer<uint32_t> onNodeRemoved;
    Observer<uint32_t, uint32_t, uint32_t, uint32_t> onConnectionAdded; // source_id, source_index, dest_id, dest_index
    Observer<uint32_t, uint32_t, uint32_t> onConnectionRemoved; // source_id, dest_id, dest_index
    Observer<std::shared_ptr<Mesh>> onMeshUpdate;

    size_t size() const noexcept { return m_nodes.size(); }
    void addNode(const std::shared_ptr<Node>& t_node);
    void cookNode(const std::shared_ptr<Node> &t_node, const uint32_t t_index = 0);
    void removeNode(const std::string& t_name);
    std::shared_ptr<Node> node(const std::string& t_name) const noexcept;

    std::shared_ptr<Node> renderNode() { return m_currentRenderNode.lock(); }
    
private:
    std::string findName(const std::string &t_name) const;
    uint32_t findId() const;

    std::unordered_map<std::uint32_t, std::shared_ptr<Node>> m_nodes;
    std::unordered_map<std::string, std::uint32_t> m_nodesIds;

    std::weak_ptr<Node> m_currentRenderNode;

};

}