#include "node_graph_menu.h"

#include "nodes/nodes_info.h"

#include "interface/utils/string_utils.h"

namespace butter {

void NodeGraphMenu::render() {
    if (ImGui::BeginPopup("NodeGraphContextMenu")) {
        const auto& menuItems = NodesInfo::getMenuItems();

        for (auto& [menuName, items] : menuItems) {
            for (const auto& item : items) {
                if (ImGui::MenuItem(item.name.c_str(), nullptr)) {
                    if (auto graph = m_graph.lock()) {
                        graph->addNode(item.createNode());
                    }
                }
            }
        }
        ImGui::EndPopup();
    }
}

void NodeGraphMenu::beginStyle() const {
    ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(70, 70, 70, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
}

void NodeGraphMenu::endStyle() const {
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

} // namespace butter