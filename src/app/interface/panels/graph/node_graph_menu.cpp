#include "node_graph_menu.h"

#include "nodes/nodes_info.h"

#include "interface/utils/string_utils.h"

namespace butter {

void NodeGraphMenu::render() {
    beginStyle();

    if (ImGui::BeginPopup("NodeGraphContextMenu")) {
        const auto& menuItems = NodesInfo::getMenuItems();

        drawSearchBar();

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
    } else {
        // Reset focus
        m_focusSearchBar = true;
    }

    endStyle();
}

void NodeGraphMenu::beginStyle() const {
    ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(70, 70, 70, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
}

void NodeGraphMenu::endStyle() const {
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

void NodeGraphMenu::drawSearchBar() {
    ImGui::PushStyleColor(ImGuiCol_FrameBg, SEARCH_BAR_COLOR);

    if (m_focusSearchBar) {
        ImGui::SetKeyboardFocusHere();
        m_focusSearchBar = false;
    }

    if (ImGui::InputTextWithHint("##searchBar", "Search", (char*)m_searchText.c_str(), m_searchText.capacity() + 1,
                                 ImGuiInputTextFlags_CallbackResize, StringImGuiCallBack, (void*)&m_searchText)) {
    }
    ImGui::PopStyleColor();
}

} // namespace butter