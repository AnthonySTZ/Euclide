#include "node_graph_input_handler.h"

namespace butter {

void NodeGraphInputHandler::pollEvents() {
    handleContextMenu();
}

void NodeGraphInputHandler::handleContextMenu() {
    ImVec2 region = ImGui::GetContentRegionAvail();
    ImGui::InvisibleButton("##nodegraph_bg", region, ImGuiButtonFlags_None);

    // Right click inside graph area -> open menu
    if (ImGui::IsItemHovered() &&
        (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || ImGui::IsKeyPressed(ImGuiKey_Tab))) {
        ImGui::OpenPopup("NodeGraphContextMenu");
    }
}

} // namespace butter
