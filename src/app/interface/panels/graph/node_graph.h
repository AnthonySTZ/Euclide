#pragma once

#include "node_item.h"
#include "connection_item.h"

#include "nodes/scene.h"

#include <unordered_map>
#include <vector>
#include <unordered_set>

namespace butter {

enum class IOType { INPUT, OUTPUT };

struct IOInfos {
    uint32_t nodeId = 0;
    IOType type = IOType::INPUT;
    uint32_t index = 0;
};

class NodeGraph {
  public:
    Observer<std::weak_ptr<Node>> onNodeSelected;

    NodeGraph(const std::shared_ptr<Scene> t_scene);
    ~NodeGraph() = default;

    void addNode(const std::shared_ptr<Node>& t_node);
    void addConnection(const IOInfos& t_first, const IOInfos& t_second) const;
    void removeConnection(const size_t t_connectionIndex);
    [[nodiscard]] inline std::weak_ptr<NodeItem> getNode(const uint32_t t_nodeId) const noexcept {
        auto it = nodes.find(t_nodeId);
        if (it == nodes.end())
            return std::weak_ptr<NodeItem>();
        return it->second;
    }
    [[nodiscard]] inline std::unordered_set<uint32_t> selectedNodes() const noexcept { return m_selectedNodes; }
    void addNodeToSelection(const uint32_t t_nodeId, const bool t_removeIfAlreadySelected);
    void clearSelection();
    void renderSelectedNode();

  public:
    std::vector<ConnectionItem> connections;
    std::unordered_map<uint32_t, std::shared_ptr<NodeItem>> nodes;
    std::vector<uint32_t> drawNodesOrder{};

  private:
    void onNodeAdded(const uint32_t t_nodeId, const std::shared_ptr<Node> t_node);
    void onNodeRemoved(const uint32_t t_nodeId);
    void onConnectionAdded(const uint32_t t_sourceId, const uint32_t t_sourceIndex, const uint32_t t_destId,
                           const uint32_t t_destIndex);
    void onConnectionRemoved(const uint32_t t_sourceId, const uint32_t t_destId, const uint32_t t_destIndex);

  private:
    std::unordered_set<uint32_t> m_selectedNodes;
    std::weak_ptr<NodeItem> m_lastNodeSelected;
    std::weak_ptr<Scene> m_scene;
};

} // namespace butter