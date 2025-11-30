#include "node_graph_input_handler.h"

namespace butter {

void NodeGraphInputHandler::pollEvents() {
    handleContextMenu();
}

void NodeGraphInputHandler::handleContextMenu() {
    ImVec2 regionAvail = ImGui::GetContentRegionAvail();
    ImVec2 region = ImVec2(std::max(regionAvail.x, 100.0f), std::max(regionAvail.y, 100.0f));
    ImGui::InvisibleButton("##nodegraph_bg", region, ImGuiButtonFlags_None);

    // Right click inside graph area -> open menu
    if (ImGui::IsItemHovered() &&
        (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || ImGui::IsKeyPressed(ImGuiKey_Tab))) {
        ImGui::OpenPopup("NodeGraphContextMenu");
    }
}

} // namespace butter
