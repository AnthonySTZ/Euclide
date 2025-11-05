#pragma once

#include "nodes/node.h"

#include "interface/utils/imgui_utils.h"
#include <memory>

namespace butter {

class NodeItem {

public:
    NodeItem(const std::shared_ptr<Node>& t_node, const ImVec2 t_position);

    void draw();
    
private:
    void drawRect(const std::string &t_nodeName);
 
    std::weak_ptr<Node> m_node;
    ImVec2 m_position;
	ImVec2 m_size = ImVec2(100, 30);
	ImU32 m_color = IM_COL32(100, 100, 100, 255);
    ImU32 m_outlineColor = IM_COL32(200, 200, 200, 255);
};


}