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
        handleLeftMouseClicked();
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

    if (m_currentConnection) {
        repositionCurrentConnection();
        m_currentConnection->draw();
    }
}

void NodeGraphInputHandler::repositionCurrentConnection() {
    auto graph = m_graph.lock();
    if (!graph)
        return;

    ImVec2 mousePos = ImGui::GetMousePos();
    auto nearIO = NodeGraphInteraction::getNodeIOAt(graph, mousePos, BIG_IO_RADIUS);
    if (nearIO.has_value()) {
        m_currentConnection->setUnconnectedPosition(nearIO.value().position);
    } else {
        m_currentConnection->setUnconnectedPosition(mousePos);
    }
}

void NodeGraphInputHandler::handleLeftMouseClicked() {
    ImVec2 mousePos = ImGui::GetMousePos();
    m_mouseButtonLeftDown = true;

    m_ioClicked = NodeGraphInteraction::getNodeIOAt(m_graph, mousePos, IO_RADIUS);
    if (m_ioClicked.has_value()) { // Check if user clicked on IO
        startConnection(m_ioClicked.value());
        return;
    }

    m_draggingNode = NodeGraphInteraction::getNodeAt(m_graph, mousePos);
    if (m_draggingNode.has_value()) // Check if user clicked on a Node
        return;

    m_isBoxSelecting = true;
    m_boxStart = mousePos;
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

void NodeGraphInputHandler::startConnection(const IOInfos& t_infos) {
    auto graph = m_graph.lock();
    if (!graph)
        return;

    ImVec2 mousePos = ImGui::GetMousePos();
    m_currentConnection = std::make_unique<ConnectionItem>();
    if (t_infos.type == IOType::INPUT) {
        m_currentConnection->setDestination(graph->getNode(t_infos.nodeId), t_infos.index);
    } else {
        m_currentConnection->setSource(graph->getNode(t_infos.nodeId), t_infos.index);
    }
    m_currentConnection->setUnconnectedPosition(mousePos);
}

void NodeGraphInputHandler::endConnection() {
    m_currentConnection = nullptr;
}

void NodeGraphInputHandler::handleLeftMouseRelease() const {
    auto graph = m_graph.lock();
    if (!graph)
        return;

    if (m_ioClicked.has_value()) {
        std::optional<IOInfos> ioReleased =
            NodeGraphInteraction::getNodeIOAt(m_graph, ImGui::GetMousePos(), BIG_IO_RADIUS);
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
        auto model = node->model();
        if (!model->isSelected()) {
            model->moveBy(t_dragDelta);
            return;
        }
    }

    for (auto nodeId : graph->selectedNodes()) {
        if (auto node = graph->getNode(nodeId).lock())
            node->model()->moveBy(t_dragDelta);
    }
}

void NodeGraphInputHandler::resetMouseInteraction() {
    m_mouseButtonLeftDown = false;
    m_isBoxSelecting = false;
    m_isMouseDrag = false;
    m_draggingNode = std::nullopt;
    m_ioClicked = std::nullopt;
    endConnection();
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

    if (ImGui::IsKeyReleased(ImGuiKey_F) && m_isWindowHovered) {
        recenterGraph();
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

void NodeGraphInputHandler::recenterGraph() {
    auto graph = m_graph.lock();
    if (!graph)
        return;

    std::vector<uint32_t> selectedNodes = NodeGraphInteraction::getSelectedNodes(m_graph);

    if (selectedNodes.empty())
        return;

    ImVec2 center = graph->nodes[selectedNodes[0]]->model()->position();
    for (size_t i = 1; i < selectedNodes.size(); ++i) {
        auto model = graph->nodes[selectedNodes[i]]->model();
        center += model->position() + model->size() * 0.5f;
    }

    const float factor = 1.0f / selectedNodes.size();
    center *= factor;

    ImVec2 centerWindow = ImGui::GetWindowPos() + ImGui::GetWindowSize() * 0.5f;
    ImVec2 moveOffset = centerWindow - center;
    for (auto& [_, nodeItem] : graph->nodes) {
        nodeItem->model()->moveBy(moveOffset);
    }
}

} // namespace euclide
