#include "node_graph_input_handler.h"

#include "node_graph_menu.h"

namespace butter {

void NodeGraphInputHandler::pollEvents() {
    handleContextMenu();
    handleMouseInputs();
}

void NodeGraphInputHandler::handleContextMenu() {
    ImVec2 regionAvail = ImGui::GetContentRegionAvail();
    ImVec2 region = ImVec2(std::max(regionAvail.x, 100.0f), std::max(regionAvail.y, 100.0f));
    ImGui::InvisibleButton("##nodegraph_bg", region, ImGuiButtonFlags_None);

    // Right click inside graph area -> open menu
    if (ImGui::IsItemHovered() &&
        (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || ImGui::IsKeyPressed(ImGuiKey_Tab))) {
        ImGui::OpenPopup(NodeGraphMenu::CONTEXT_MENU_NAME.data());
    }
}

void NodeGraphInputHandler::handleMouseInputs() {
    if (ImGui::IsPopupOpen(NodeGraphMenu::CONTEXT_MENU_NAME.data()))
        return;

    const bool isWindowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && isWindowHovered) {
        m_mouseButtonLeftDown = true;
        m_draggingNode = m_graphRenderer->getNodeAt(ImGui::GetMousePos());
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        handleLeftMouseRelease();

        m_mouseButtonLeftDown = false;
        m_isMouseDrag = false;
    }

    if (m_isMouseDrag) {
        handleDragging();
    } else if (!m_isMouseDrag && m_mouseButtonLeftDown) {
        if (isDragging()) {
            m_isMouseDrag = true;
        }
    }
}

bool NodeGraphInputHandler::isDragging() const {
    ImVec2 dragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left, DRAG_THRESHOLD);
    return (dragDelta.x != 0 || dragDelta.y != 0);
}

void NodeGraphInputHandler::handleDragging() const {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 dragDelta = io.MousePos - io.MousePosPrev;
    if (m_draggingNode.has_value()) {
        dragNodes(dragDelta);
    } else {
        // TODO: Box selection for example
    }
}

void NodeGraphInputHandler::handleLeftMouseRelease() const {
    auto graph = m_graph.lock();
    if (!graph)
        return;

    if (m_draggingNode.has_value() && !m_isMouseDrag) {
        const uint32_t id = m_draggingNode.value();
        if (!ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
            graph->clearSelection();
        }
        graph->addNodeToSelection(id, false);
        return;
    }

    if (!m_isMouseDrag) {
        graph->clearSelection();
        return;
    }

    // TODO: Box Selection
}

void NodeGraphInputHandler::dragNodes(const ImVec2& t_dragDelta) const {
    auto graph = m_graph.lock();
    if (!graph)
        return;

    const uint32_t id = m_draggingNode.value();
    if (auto node = graph->getNode(id).lock()) {
        if (!node->isSelected()) {
            node->moveBy(t_dragDelta);
            return;
        }
    }

    for (auto nodeId : graph->selectedNodes()) {
        if (auto node = graph->getNode(nodeId).lock())
            node->moveBy(t_dragDelta);
    }
}

} // namespace butter
