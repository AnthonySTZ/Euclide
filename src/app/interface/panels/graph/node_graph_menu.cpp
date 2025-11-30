#include "node_graph_menu.h"

#include "interface/utils/string_utils.h"

namespace butter {

void NodeGraphMenu::render() {
    beginStyle();

    if (ImGui::BeginPopup("NodeGraphContextMenu")) {
        const auto& menuItems = NodesInfo::getMenuItems();

        drawSearchBar();

        for (auto& [menuName, items] : menuItems) {
            drawItems(menuName, items);
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

void NodeGraphMenu::drawItems(const std::string& t_menu, const std::vector<NodeMenuItem>& t_items) {
    if (m_searchText.empty()) {
        // Draw Nodes with categories
        if (ImGui::BeginMenu(t_menu.c_str())) {
            for (const auto& item : t_items) {
                drawItem(item);
            }
            ImGui::Separator();
            ImGui::EndMenu();
        }
        return;
    }

    for (const auto& item : t_items) {
        if (toLower(item.name).find(toLower(m_searchText)) == std::string::npos)
            continue;
        drawItem(item);
    }
}

void NodeGraphMenu::drawItem(const NodeMenuItem& t_item) {
    if (ImGui::MenuItem(t_item.name.c_str(), nullptr)) {
        if (auto graph = m_graph.lock()) {
            graph->addNode(t_item.createNode());
        }
    }
}

} // namespace butter