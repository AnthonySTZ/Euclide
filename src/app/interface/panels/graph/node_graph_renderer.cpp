#include "node_graph_renderer.h"

namespace butter {

void NodeGraphRenderer::render() const {
    drawConnections();
    drawCuttingLines();
    drawNodes();
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

void NodeGraphRenderer::drawCuttingLines() const {
    for (auto& line : m_cuttingsLines)
        line.draw();
}

void NodeGraphRenderer::drawBoxSelection(const ImVec2& t_startPos, const ImVec2& t_endPos) const {
    ImDrawList* drawlist = ImGui::GetForegroundDrawList();
    if (!drawlist)
        return;

    ImVec2 start = t_startPos;
    ImVec2 end = t_endPos;

    if (t_startPos.x > t_endPos.x) {
        start.x = t_endPos.x;
        end.x = t_startPos.x;
    }
    if (t_startPos.y > t_endPos.y) {
        start.y = t_endPos.y;
        end.y = t_startPos.y;
    }

    drawlist->AddRectFilled(start, end, SELECTION_BOX_COLOR, SELECTION_BOX_ROUNDING);
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
