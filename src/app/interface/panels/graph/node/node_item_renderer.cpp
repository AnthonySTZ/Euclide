#include "node_item_renderer.h"

namespace euclide {

void NodeItemRenderer::draw(const NodeItem& t_nodeItem) {
    drawRect(t_nodeItem);
}

void NodeItemRenderer::drawRect(const NodeItem& t_nodeItem) {
    auto node = t_nodeItem.m_node.lock();
    if (!node)
        return;

    const std::string nodeName = node->name();
    const bool isRender = node->isRender();
    const ImU32 rectColor = isRender ? t_nodeItem.m_renderColor : t_nodeItem.m_color;
    const ImU32 outlineRectColor =
        t_nodeItem.m_isSelected ? t_nodeItem.m_selectedOutlineColor : t_nodeItem.m_outlineColor;

    ImVec2 nodeEnd = t_nodeItem.m_position + t_nodeItem.m_size;
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    drawList->AddRectFilled(t_nodeItem.m_position, nodeEnd, rectColor, 4.0f);
    drawList->AddRect(t_nodeItem.m_position, nodeEnd, outlineRectColor, 4.0f, 0, 2.0f);

    ImVec2 textSize = ImGui::CalcTextSize(nodeName.c_str());
    ImVec2 textPos = t_nodeItem.m_position;
    textPos.x += t_nodeItem.m_size.x * 0.5f - textSize.x * 0.5f;
    textPos.y += t_nodeItem.m_size.y * 0.5f - textSize.y * 0.5f;
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), nodeName.c_str());
}

} // namespace euclide