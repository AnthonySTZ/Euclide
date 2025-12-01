#include "node_graph_renderer.h"

namespace butter {

void NodeGraphRenderer::render() const {
    drawConnections();
    drawNodes();
}

std::optional<uint32_t> NodeGraphRenderer::getNodeAt(ImVec2 t_mousePosition) const {
    auto graph = m_graph.lock();
    if (!graph)
        return std::nullopt;

    for (auto it = graph->drawNodesOrder.rbegin(); it != graph->drawNodesOrder.rend(); ++it) {
        auto nodeItem = graph->nodes[*it];
        if (nodeItem->isHovered()) {
            return *it;
        }
    }
    return std::nullopt;
}

void NodeGraphRenderer::drawConnections() const {
    auto graph = m_graph.lock();
    if (!graph)
        return;

    for (ConnectionItem& conn : graph->connections) {
        conn.draw();
    }
}

void NodeGraphRenderer::drawNodes() const {
    auto graph = m_graph.lock();
    if (!graph)
        return;

    for (const uint32_t id : graph->drawNodesOrder) {
        if (auto node = graph->getNode(id).lock()) {
            node->draw();
        }
    }
}

} // namespace butter
