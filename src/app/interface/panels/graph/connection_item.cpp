#include "connection_item.h"

namespace euclide {

void ConnectionItem::draw() {
    if (const auto sourceNode = m_sourceNode.lock()) {
        m_start = NodeItemInteraction::getOutputIOPosition(sourceNode, m_sourceIndex);
    }
    if (const auto destNode = m_destNode.lock()) {
        m_end = NodeItemInteraction::getInputIOPosition(destNode, m_destIndex);
    }

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddLine(m_start, m_end, COLOR, THICKNESS);
}

void ConnectionItem::setSource(const std::weak_ptr<NodeItem> t_sourceNode, const uint32_t t_sourceIndex) noexcept {
    m_sourceNode = t_sourceNode;
    m_sourceIndex = t_sourceIndex;
}

void ConnectionItem::setDestination(const std::weak_ptr<NodeItem> t_destNode, const uint32_t t_destIndex) noexcept {
    m_destNode = t_destNode;
    m_destIndex = t_destIndex;
}

void ConnectionItem::setUnconnectedPosition(const ImVec2& t_position) noexcept {
    if (const auto sourceNode = m_sourceNode.lock()) {
        m_end = t_position;
    } else {
        m_start = t_position;
    }
}

void ConnectionItem::deleteConnection() {
    if (auto destNode = m_destNode.lock()) {
        destNode->model()->node()->deleteInputConnection(m_destIndex);
    }
}

} // namespace euclide
