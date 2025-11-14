#include "parameters.h"

#include "interface/panels/fields/field_drawer.h"

namespace butter {

Parameters::Parameters(const std::shared_ptr<NodeGraph> &t_nodeGraph)
    : m_nodeGraph(t_nodeGraph)
{
    if (auto nodeGraph = m_nodeGraph.lock()){
        nodeGraph->onNodeSelected.subscribe([this](std::weak_ptr<Node> t_node) {
            m_node = t_node;
        });
    }
}

void Parameters::draw()
{
    beginTab("Parameters", m_padding);
    
    pushStyle();
    drawParameters();
    popStyle();

    endTab();
}

void Parameters::drawParameters() {
    FieldDrawer drawer;
    if (auto node = m_node.lock()) {
        for(const auto& [name, field]: node->fields()) {
            field->accept(name, drawer);
        }
    }
}

void Parameters::pushStyle() {
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(90, 90, 90, 255));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 70, 255));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(60, 60, 60, 255));

    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(90, 90, 90, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(70, 70, 70, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(60, 60, 60, 255));

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
}

void Parameters::popStyle() {
    ImGui::PopStyleColor(6);
    ImGui::PopStyleVar(1);
}

}

