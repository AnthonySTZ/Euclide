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
    int inputIOHovered() const;
    int outputIOHovered() const;

    void moveBy(const ImVec2& t_delta);
    void setSelected(const bool t_isSelected) { m_isSelected = t_isSelected; }
    bool isSelected() const noexcept { return m_isSelected; }

    ImVec2 getInputIOPosition(uint32_t index) const;
    ImVec2 getOutputIOPosition(uint32_t index) const;

    std::shared_ptr<Node> node() { return m_node.lock(); }
    
private:
    void drawRect(const std::string &t_nodeName);
    void drawIOs(const int t_numberOfInputs, const int t_numberOfOutputs);
    int isIOsHovered(const std::vector<ImVec2> &t_ioPositions) const;

    std::vector<ImVec2> drawIOsOnLine(const int t_numberOfIOs, ImVec2 t_linePosition);

    std::weak_ptr<Node> m_node;
    bool m_isSelected = false;

    ImVec2 m_position;
    std::vector<ImVec2> m_inputIOPositions;
    std::vector<ImVec2> m_outputIOPositions;

	ImVec2 m_size = ImVec2(100, 30);
	ImU32 m_color = IM_COL32(100, 100, 100, 255);
    ImU32 m_outlineColor = IM_COL32(200, 200, 200, 255);
    ImU32 m_selectedOutlineColor = IM_COL32(255, 178, 102, 255);

    static constexpr float s_radius = 5.5;
    static constexpr float s_radius2 = s_radius * s_radius;
    static constexpr float s_spacing = 3.0;
	static constexpr ImU32 s_ioOutlineColor =  IM_COL32(200, 200, 200, 255);
    static constexpr ImU32 s_ioColor = IM_COL32(170, 170, 170, 255);
};


}