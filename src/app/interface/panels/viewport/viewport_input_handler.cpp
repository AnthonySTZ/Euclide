#include "viewport_input_handler.h"

namespace euclide {

void ViewportInputHandler::pollEvents() {
    m_isWindowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

    handleMouseInputs();
    handleKeyInputs();
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

void ViewportInputHandler::handleKeyInputs() {
    auto viewport = m_viewport.lock();
    if (!m_isWindowHovered || !viewport)
        return;

    if (ImGui::IsKeyPressed(ImGuiKey_P))
        viewport->renderModel().tooglePrimitives();
    if (ImGui::IsKeyPressed(ImGuiKey_W))
        viewport->renderModel().toogleWireframe();
    if (ImGui::IsKeyPressed(ImGuiKey_V))
        viewport->renderModel().tooglePoints();
    if (ImGui::IsKeyPressed(ImGuiKey_G))
        viewport->gridModel().toogleWireframe();
    if (ImGui::IsKeyPressed(ImGuiKey_F))
        viewport->retargetCamera();
}

} // namespace euclide