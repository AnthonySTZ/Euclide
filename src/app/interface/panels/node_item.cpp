#include "node_item.h"

namespace butter {
    
NodeItem::NodeItem(const std::shared_ptr<Node> &t_node, const ImVec2 t_position)
    : m_node(t_node), m_position(t_position)
{}

void NodeItem::draw(const bool t_isSelected)
{
    if (auto node = m_node.lock()) {
        drawRect(node->name(), t_isSelected);
    }
}

bool NodeItem::isHovered() const
{
    ImGuiIO& io = ImGui::GetIO();
	ImVec2 nodeEnd = m_position + m_size;

	return io.MousePos.x >= m_position.x && io.MousePos.x <= nodeEnd.x &&
		io.MousePos.y >= m_position.y && io.MousePos.y <= nodeEnd.y;
}

void NodeItem::moveBy(const ImVec2& t_delta)
{
    m_position += t_delta;
}

void NodeItem::drawRect(const std::string& t_nodeName, const bool t_isSelected) {
    ImVec2 nodeEnd = m_position + m_size;
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	drawList->AddRectFilled(m_position, nodeEnd, m_color, 4.0f);
	drawList->AddRect(m_position, nodeEnd, t_isSelected ? m_selectedOutlineColor : m_outlineColor, 4.0f, 0, 2.0f);

	ImVec2 textSize = ImGui::CalcTextSize(t_nodeName.c_str());
	ImVec2 textPos = m_position;
	textPos.x += m_size.x * 0.5f - textSize.x * 0.5f;
	textPos.y += m_size.y * 0.5f - textSize.y * 0.5f;
	drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), t_nodeName.c_str());
}

}