#include "panel.h"

namespace euclide {

void Panel::beginTab(const char* t_name, ImVec2 t_padding, ImU32 t_bgCol) const {
    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&window_class);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, t_padding);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, t_bgCol);
    ImGui::Begin(t_name);
    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(1);

    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 titleRectSize = ImVec2(windowSize.x, TAB_TITLE_HEIGHT);
    ImVec2 endTitleBar = windowPos;
    endTitleBar.y = titleRectSize.y;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(windowPos, windowPos + titleRectSize, TAB_TITLE_BG_COLOR);

    draw_list->AddText(windowPos + TAB_TITLE_TEXT_PADDING, TAB_TITLE_TEXT_COLOR, t_name);

    ImVec2 newCursorPos = ImVec2(0.0, titleRectSize.y) + t_padding;
    ImGui::GetWindowDrawList()->PushClipRect(windowPos + newCursorPos, windowPos + windowSize);

    ImGui::SetCursorPos(newCursorPos);
}

void Panel::endTab() const {
    ImGui::End();
}

} // namespace euclide