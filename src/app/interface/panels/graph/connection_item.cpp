#include "connection_item.h"

namespace butter {

void ConnectionItem::draw() const
{
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 m_start = io.MousePos;
    ImVec2 m_end = io.MousePos;

    if (auto sourceNode = m_sourceNode.lock()) {
        m_start = sourceNode->getOutputIOPosition(m_sourceIndex);
    }
    if (auto destNode = m_destNode.lock()) {
        m_end = destNode->getInputIOPosition(m_destIndex);
    }

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddLine(m_start, m_end, s_color, s_thickness);
}

void ConnectionItem::setSource(const std::shared_ptr<NodeItem> &t_sourceNode, const uint32_t t_sourceIndex)
{
    m_sourceNode = t_sourceNode;
    m_sourceIndex = t_sourceIndex;
}

void ConnectionItem::setDestination(const std::shared_ptr<NodeItem> &t_destNode, const uint32_t t_destIndex)
{
    m_destNode = t_destNode;
    m_destIndex = t_destIndex;
}

}
