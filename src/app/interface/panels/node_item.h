#pragma once

#include "nodes/node.h"

#include "interface/utils/imgui_utils.h"
#include <memory>

namespace butter {

class NodeItem {

public:
    NodeItem(const std::shared_ptr<Node>& t_node, const ImVec2 t_position);

    void draw();
    bool isHovered() const;

    void moveBy(const ImVec2& t_delta);
    void setSelected(const bool t_isSelected) { m_isSelected = t_isSelected; }

private:
    void drawRect(const std::string &t_nodeName);
 
    std::weak_ptr<Node> m_node;
    bool m_isSelected = false;

    ImVec2 m_position;
	ImVec2 m_size = ImVec2(100, 30);
	ImU32 m_color = IM_COL32(100, 100, 100, 255);
    ImU32 m_outlineColor = IM_COL32(200, 200, 200, 255);
    ImU32 m_selectedOutlineColor = IM_COL32(255, 178, 102, 255);
};


}