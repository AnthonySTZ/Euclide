#include "node_graph.h"

namespace butter {

NodeGraph::NodeGraph(const std::shared_ptr<Scene> t_scene) : m_scene(t_scene) {
    auto scene = m_scene.lock();
    if (!scene)
        return;

    scene->onNodeAdded.subscribe(
        [this](uint32_t t_nodeId, std::shared_ptr<Node> t_node) { onNodeAdded(t_nodeId, t_node); });

    scene->onNodeRemoved.subscribe([this](uint32_t t_nodeId) { onNodeRemoved(t_nodeId); });

    scene->onConnectionAdded.subscribe(
        [this](const uint32_t t_sourceId, const uint32_t t_sourceIndex, const uint32_t t_destId,
               const uint32_t t_destIndex) { onConnectionAdded(t_sourceId, t_sourceIndex, t_destId, t_destIndex); });

    scene->onConnectionRemoved.subscribe([this](uint32_t t_sourceId, uint32_t t_destId, uint32_t t_destIndex) {
        onConnectionRemoved(t_sourceId, t_destId, t_destIndex);
    });
}

void NodeGraph::addNode(const std::shared_ptr<Node>& t_node) {
    auto scene = m_scene.lock();
    if (!scene)
        return;

    scene->addNode(t_node);
}

void NodeGraph::addNodeToSelection(const uint32_t t_nodeId, const bool t_removeIfAlreadySelected) {
    auto nodeItem = getNode(t_nodeId).lock();
    if (!nodeItem)
        return;

    if (t_removeIfAlreadySelected) {
        if (m_selectedNodes.find(t_nodeId) != m_selectedNodes.end()) { // Remove Node Item if its already selected
            m_selectedNodes.erase(t_nodeId);
            nodeItem->setSelected(false);
            return;
        }
    }

    m_selectedNodes.insert(t_nodeId);
    nodeItem->setSelected(true);
    onNodeSelected.notify(nodeItem->node());
}

void NodeGraph::clearSelection() {
    for (auto nodeId : m_selectedNodes) {
        if (auto node = getNode(nodeId).lock()) {
            node->setSelected(false);
        }
    }
    m_selectedNodes.clear();
    onNodeSelected.notify(std::weak_ptr<Node>());
}

void NodeGraph::onNodeAdded(const uint32_t t_nodeId, const std::shared_ptr<Node> t_node) {
    ImGuiIO& io = ImGui::GetIO();
    auto [_, inserted] = nodes.try_emplace(t_nodeId, std::make_shared<NodeItem>(t_node, io.MousePos));
    if (inserted)
        drawNodesOrder.push_back(t_nodeId);
}

void NodeGraph::onNodeRemoved(const uint32_t t_nodeId) {
    if (nodes.find(t_nodeId) == nodes.end())
        return;
    nodes.erase(t_nodeId);
    drawNodesOrder.erase(std::remove(drawNodesOrder.begin(), drawNodesOrder.end(), t_nodeId), drawNodesOrder.end());
}

void NodeGraph::onConnectionAdded(const uint32_t t_sourceId, const uint32_t t_sourceIndex, const uint32_t t_destId,
                                  const uint32_t t_destIndex) {
    ConnectionItem conn{};
    conn.setSource(nodes[t_sourceId], t_sourceIndex);
    conn.setDestination(nodes[t_destId], t_destIndex);
    connections.push_back(conn);
}

void NodeGraph::onConnectionRemoved(const uint32_t t_sourceId, const uint32_t t_destId, const uint32_t t_destIndex) {
    auto itSrc = nodes.find(t_sourceId);
    auto itDst = nodes.find(t_destId);

    if (itSrc == nodes.end() || itDst == nodes.end())
        return;

    const auto srcNode = itSrc->second;
    const auto dstNode = itDst->second;

    connections.erase(std::remove_if(connections.begin(), connections.end(),
                                     [&](const ConnectionItem& conn) {
                                         return conn.sourceNode() == srcNode && conn.destinationNode() == dstNode &&
                                                conn.destinationIndex() == t_destIndex;
                                     }),
                      connections.end());
}

} // namespace butter