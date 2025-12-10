#include "node_item_renderer.h"

namespace euclide {

NodeItemRenderer::NodeItemRenderer(const std::weak_ptr<NodeItemModel> t_model) : m_nodeModel(t_model) {
}

void NodeItemRenderer::render() {
    auto nodeModel = m_nodeModel.lock();
    if (!nodeModel)
        return;

    drawRect(nodeModel);
    drawIOs(nodeModel);
}

void NodeItemRenderer::drawRect(const std::shared_ptr<NodeItemModel>& t_nodeModel) const {
    auto node = t_nodeModel->node();
    if (!node)
        return;

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    if (!drawList)
        return;

    // ------ Draw Rect ------ //
    const ImVec2 position = t_nodeModel->m_position;
    const ImVec2 size = t_nodeModel->m_size;
    const ImVec2 nodeEnd = position + size;

    const ImU32 rectColor = node->isRender() ? t_nodeModel->m_renderColor : t_nodeModel->m_color;
    const ImU32 rectOutlineColor =
        t_nodeModel->m_isSelected ? t_nodeModel->m_selectedOutlineColor : t_nodeModel->m_outlineColor;

    drawList->AddRectFilled(position, nodeEnd, rectColor, 4.0f);
    drawList->AddRect(position, nodeEnd, rectOutlineColor, 4.0f, 0, 2.0f);

    // ------ Draw Node Name ------ //
    const std::string nodeName = node->name();
    const ImVec2 textSize = ImGui::CalcTextSize(nodeName.c_str());
    const ImVec2 textPos{
        position.x + size.x * 0.5f - textSize.x * 0.5f,
        position.y + size.y * 0.5f - textSize.y * 0.5f,
    };
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), nodeName.c_str());
}

} // namespace euclide