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

std::optional<IOInfos> NodeGraphRenderer::getNodeIOAt(ImVec2 t_mousePosition) const {
    auto graph = m_graph.lock();
    if (!graph)
        return std::nullopt;

    for (auto [id, nodeItem] : graph->nodes) {
        std::optional<uint32_t> inputIndex = nodeItem->inputIOHovered();
        if (inputIndex.has_value()) {
            return IOInfos{id, IOType::INPUT, inputIndex.value()};
        }

        std::optional<uint32_t> outputIndex = nodeItem->outputIOHovered();
        if (outputIndex.has_value()) {
            return IOInfos{id, IOType::OUTPUT, outputIndex.value()};
        }
    }

    return std::nullopt;
}

void NodeGraphRenderer::startConnection(const IOInfos& t_infos) {
    auto graph = m_graph.lock();
    if (!graph)
        return;

    m_currentConnection = std::make_unique<ConnectionItem>();
    if (t_infos.type == IOType::INPUT) {
        m_currentConnection->setDestination(graph->getNode(t_infos.nodeId), t_infos.index);
    } else {
        m_currentConnection->setSource(graph->getNode(t_infos.nodeId), t_infos.index);
    }
}

void NodeGraphRenderer::endConnection() {
    m_currentConnection = nullptr;
}

void NodeGraphRenderer::drawConnections() const {
    auto graph = m_graph.lock();
    if (!graph)
        return;

    for (ConnectionItem& conn : graph->connections) {
        conn.draw();
    }

    if (m_currentConnection)
        m_currentConnection->draw();
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
