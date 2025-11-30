#include "node_item.h"

namespace butter {

NodeItem::NodeItem(const std::shared_ptr<Node>& t_node, const ImVec2 t_position)
    : m_node(t_node), m_position(t_position) {
}

void NodeItem::draw() {
    if (auto node = m_node.lock()) {
        drawRect(node->name(), node->isRender());
        drawIOs(node->numInputs(), node->numOutputs());
    }
}

bool NodeItem::isHovered() const {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 nodeEnd = m_position + m_size;

    return io.MousePos.x >= m_position.x && io.MousePos.x <= nodeEnd.x && io.MousePos.y >= m_position.y &&
           io.MousePos.y <= nodeEnd.y;
}

int NodeItem::inputIOHovered() const {
    return isIOsHovered(m_inputIOPositions);
}

int NodeItem::outputIOHovered() const {
    return isIOsHovered(m_outputIOPositions);
}

int NodeItem::isIOsHovered(const std::vector<ImVec2>& t_ioPositions) const {
    ImGuiIO& io = ImGui::GetIO();

    for (size_t i = 0; i < t_ioPositions.size(); ++i) {
        const ImVec2 diff = t_ioPositions[i] - io.MousePos;
        const float dist2 = diff.x * diff.x + diff.y * diff.y;
        if (dist2 <= RADIUS_2)
            return i;
    }

    return -1;
}

void NodeItem::moveBy(const ImVec2& t_delta) {
    m_position += t_delta;
}

ImVec2 NodeItem::getInputIOPosition(uint32_t index) const {
    const float ioSpacing = m_size.x / (m_inputIOPositions.size() + 1);
    return ImVec2{m_position.x + (index + 1) * ioSpacing, m_position.y - SPACING - RADIUS};
}

ImVec2 NodeItem::getOutputIOPosition(uint32_t index) const {
    const float ioSpacing = m_size.x / (m_outputIOPositions.size() + 1);
    return ImVec2{m_position.x + (index + 1) * ioSpacing, m_position.y + m_size.y + SPACING + RADIUS};
}

void NodeItem::drawRect(const std::string& t_nodeName, const bool isRender) {
    ImVec2 nodeEnd = m_position + m_size;
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    drawList->AddRectFilled(m_position, nodeEnd, isRender ? m_renderColor : m_color, 4.0f);
    drawList->AddRect(m_position, nodeEnd, m_isSelected ? m_selectedOutlineColor : m_outlineColor, 4.0f, 0, 2.0f);

    ImVec2 textSize = ImGui::CalcTextSize(t_nodeName.c_str());
    ImVec2 textPos = m_position;
    textPos.x += m_size.x * 0.5f - textSize.x * 0.5f;
    textPos.y += m_size.y * 0.5f - textSize.y * 0.5f;
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), t_nodeName.c_str());
}

void NodeItem::drawIOs(const int t_numberOfInputs, const int t_numberOfOutputs) {
    ImVec2 inputIOPos{m_position.x, m_position.y - SPACING - RADIUS};
    ImVec2 outputIOPos{m_position.x, m_position.y + m_size.y + SPACING + RADIUS};

    m_inputIOPositions = drawIOsOnLine(t_numberOfInputs, inputIOPos);
    m_outputIOPositions = drawIOsOnLine(t_numberOfOutputs, outputIOPos);
}

std::vector<ImVec2> NodeItem::drawIOsOnLine(const int t_numberOfIOs, ImVec2 t_linePosition) const {
    std::vector<ImVec2> positions{};
    if (t_numberOfIOs <= 0)
        return positions;

    positions.reserve(t_numberOfIOs);
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    const float ioSpacing = m_size.x / (t_numberOfIOs + 1);
    for (size_t i = 1; i < t_numberOfIOs + 1; ++i) {
        ImVec2 ioPos{t_linePosition.x + i * ioSpacing, t_linePosition.y};

        drawList->AddCircleFilled(ioPos, RADIUS, IO_COLOR);
        drawList->AddCircle(ioPos, RADIUS, IO_OUTLINE_COLOR);

        positions.push_back(ioPos);
    }
    return positions;
}

} // namespace butter