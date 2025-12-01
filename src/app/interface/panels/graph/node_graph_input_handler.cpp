#include "node_graph_input_handler.h"

#include "node_graph_menu.h"
#include "node_graph_interaction.h"

namespace euclide {

void NodeGraphInputHandler::pollEvents() {
    m_isWindowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

    handleContextMenu();
    handleMouseInputs();
    handleKeyInputs();
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

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && m_isWindowHovered) {
        ImVec2 mousePos = ImGui::GetMousePos();
        m_mouseButtonLeftDown = true;
        m_ioClicked = NodeGraphInteraction::getNodeIOAt(m_graph, mousePos);
        if (m_ioClicked.has_value()) {
            m_graphRenderer->startConnection(m_ioClicked.value());
        } else {
            m_draggingNode = NodeGraphInteraction::getNodeAt(m_graph, mousePos);
            if (!m_draggingNode.has_value()) {
                m_isBoxSelecting = true;
                m_boxStart = mousePos;
            }
        }
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        handleLeftMouseRelease();
        resetMouseInteraction();
    }

    if (m_isMouseDrag) {
        handleDragging();
    } else if (m_mouseButtonLeftDown && isDragging()) {
        m_isMouseDrag = true;
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
    } else if (m_isBoxSelecting) {
        m_graphRenderer->drawBoxSelection(m_boxStart, io.MousePos);
    }
}

void NodeGraphInputHandler::handleLeftMouseRelease() const {
    auto graph = m_graph.lock();
    if (!graph)
        return;

    if (m_ioClicked.has_value()) {
        std::optional<IOInfos> ioReleased = NodeGraphInteraction::getNodeIOAt(m_graph, ImGui::GetMousePos());
        if (ioReleased.has_value()) {
            graph->addConnection(m_ioClicked.value(), ioReleased.value());
        }
        return;
    }

    if (m_draggingNode.has_value() && !m_isMouseDrag) {
        const uint32_t id = m_draggingNode.value();
        if (!ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
            graph->clearSelection();
        }
        graph->addNodeToSelection(id, true);
        return;
    }

    if (!m_isMouseDrag && m_isWindowHovered) {
        graph->clearSelection();
        return;
    }

    if (m_isBoxSelecting) {
        if (!ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
            graph->clearSelection();
        }

        const ImVec2 endBox = ImGui::GetMousePos();
        for (const uint32_t nodeId : NodeGraphInteraction::getNodesInRect(m_graph, m_boxStart, endBox)) {
            graph->addNodeToSelection(nodeId, false);
        }
    }
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

void NodeGraphInputHandler::resetMouseInteraction() {
    m_mouseButtonLeftDown = false;
    m_isBoxSelecting = false;
    m_isMouseDrag = false;
    m_draggingNode = std::nullopt;
    m_ioClicked = std::nullopt;
    m_graphRenderer->endConnection();
}

void NodeGraphInputHandler::handleKeyInputs() {
    auto graph = m_graph.lock();
    if (!graph)
        return;

    if (ImGui::IsPopupOpen(NodeGraphMenu::CONTEXT_MENU_NAME.data()))
        return;

    if (ImGui::IsKeyReleased(ImGuiKey_R) && m_isWindowHovered) {
        graph->renderSelectedNode();
    }

    if (ImGui::IsKeyReleased(ImGuiKey_Delete) && m_isWindowHovered) {
        graph->removeSelectedNodes();
    }

    if (ImGui::IsKeyDown(ImGuiKey_Y) && m_isWindowHovered) {
        ImGuiIO& io = ImGui::GetIO();
        m_graphRenderer->addCuttingLine(io.MousePosPrev, io.MousePos);
        std::optional<uint32_t> connectionCut =
            NodeGraphInteraction::getIntersectedConnectionIndex(m_graph, io.MousePosPrev, io.MousePos);
        if (connectionCut.has_value()) {
            graph->removeConnection(connectionCut.value());
        }
    } else if (ImGui::IsKeyReleased(ImGuiKey_Y)) {
        m_graphRenderer->clearCuttingLines();
    }
}

} // namespace euclide
