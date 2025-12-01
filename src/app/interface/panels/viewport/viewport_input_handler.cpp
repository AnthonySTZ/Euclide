#include "viewport_input_handler.h"

namespace butter {

void ViewportInputHandler::pollEvents() {
    m_isWindowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

    handleMouseInputs();
}

void ViewportInputHandler::handleMouseInputs() {
    auto viewport = m_viewport.lock();
    if (!viewport)
        return;

    if (m_isWindowHovered) {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            m_isLeftMouseButtonClicked = true;
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
            m_isMiddleMouseButtonClicked = true;
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            m_isRightMouseButtonClicked = true;
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        m_isLeftMouseButtonClicked = false;
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle))
        m_isMiddleMouseButtonClicked = false;
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
        m_isRightMouseButtonClicked = false;

    ImVec2 dragDelta = ImGui::GetIO().MouseDelta;

    if (m_isLeftMouseButtonClicked) {
        ImGui::SetMouseCursor(ImGuiMouseSource_Pen);
        viewport->camera()->orbit(-dragDelta.x, -dragDelta.y);
    } else if (m_isMiddleMouseButtonClicked) {
        ImGui::SetMouseCursor(ImGuiMouseSource_Pen);
        viewport->camera()->pan(-dragDelta.x, dragDelta.y);
    } else if (m_isRightMouseButtonClicked) {
        ImGui::SetMouseCursor(ImGuiMouseSource_Pen);
        viewport->camera()->dolly(-dragDelta.y);
    }
}

} // namespace butter