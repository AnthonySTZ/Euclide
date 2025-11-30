#pragma once

#include "node_item.h"
#include "connection_item.h"

#include "nodes/scene.h"

#include <unordered_map>
#include <vector>

namespace butter {

class NodeGraph {
  public:
    Observer<std::weak_ptr<Node>> onNodeSelected;

    NodeGraph(const std::shared_ptr<Scene> t_scene);
    ~NodeGraph() = default;

  public:
    std::vector<ConnectionItem> connections;
    std::unordered_map<uint32_t, std::shared_ptr<NodeItem>> nodes;
    std::vector<uint32_t> drawNodesOrder{};

    std::weak_ptr<Scene> m_scene;

  private:
    void onNodeAdded(const uint32_t t_nodeId, const std::shared_ptr<Node> t_node);
    void onNodeRemoved(const uint32_t t_nodeId);
    void onConnectionAdded(const uint32_t t_sourceId, const uint32_t t_sourceIndex, const uint32_t t_destId,
                           const uint32_t t_destIndex);
    void onConnectionRemoved(const uint32_t t_sourceId, const uint32_t t_destId, const uint32_t t_destIndex);
};

} // namespace butter