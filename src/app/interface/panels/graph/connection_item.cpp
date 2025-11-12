#include "connection_item.h"

namespace butter {

void ConnectionItem::draw()
{
    ImGuiIO& io = ImGui::GetIO();
    m_start = io.MousePos;
    m_end = io.MousePos;

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

void ConnectionItem::deleteConnection()
{
    if (auto destNode = m_destNode.lock()) {
        destNode->node()->deleteInputConnection(m_destIndex);
    }
}

}
