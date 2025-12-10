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

void NodeItemRenderer::drawIOs(const std::shared_ptr<NodeItemModel>& t_nodeModel) {
    auto node = t_nodeModel->node();
    if (!node)
        return;

    const ImVec2 position = t_nodeModel->m_position;
    const float spacing = t_nodeModel->SPACING;
    const float radius = t_nodeModel->RADIUS;
    const float nodeWidth = t_nodeModel->m_size.x;
    const ImU32 color = t_nodeModel->IO_COLOR;
    const ImU32 outlineColor = t_nodeModel->IO_OUTLINE_COLOR;

    ImVec2 inputIOPos{position.x, position.y - spacing - radius};
    ImVec2 outputIOPos{position.x, position.y + t_nodeModel->m_size.y + spacing + radius};

    m_inputIOPositions = computePointsPositionsOnLine(node->numInputs(), inputIOPos, nodeWidth);
    m_outputIOPositions = computePointsPositionsOnLine(node->numOutputs(), outputIOPos, nodeWidth);

    for (const ImVec2& ioPos : m_inputIOPositions) {
        drawIO(ioPos, radius, color, outlineColor);
    }
    for (const ImVec2& ioPos : m_outputIOPositions) {
        drawIO(ioPos, radius, color, outlineColor);
    }
}

void NodeItemRenderer::drawIO(const ImVec2 t_position, const float t_ioRadius, const ImU32 t_color,
                              const ImU32 t_outlineColor) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    if (!drawList)
        return;

    drawList->AddCircleFilled(t_position, t_ioRadius, t_color);
    drawList->AddCircle(t_position, t_ioRadius, t_outlineColor);
}

std::vector<ImVec2> NodeItemRenderer::computePointsPositionsOnLine(const int t_numberOfPoints,
                                                                   const ImVec2 t_linePosition,
                                                                   const float t_lineWidth) {
    std::vector<ImVec2> positions{};
    if (t_numberOfPoints <= 0)
        return positions;

    positions.resize(t_numberOfPoints);
    const float pointsSpacing = t_lineWidth / (t_numberOfPoints + 1);
    for (size_t i = 0; i < t_numberOfPoints; ++i) {
        positions[i] = ImVec2{t_linePosition.x + (i + 1) * pointsSpacing, t_linePosition.y};
    }
    return positions;
}

} // namespace euclide