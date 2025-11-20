#include "node_graph.h"

#include "interface/utils/string_utils.h"

namespace butter {

NodeGraph::NodeGraph(const std::shared_ptr<Scene>& t_scene)
    : m_scene(t_scene)
{
    
    if (auto scene = m_scene.lock()) {
        scene->onNodeAdded.subscribe(
            [this](uint32_t t_nodeId, std::shared_ptr<Node> t_node) {
                ImGuiIO& io = ImGui::GetIO();
                m_nodeItems.try_emplace(t_nodeId, std::make_shared<NodeItem>(t_node, io.MousePos));
                m_drawOrder.push_back(t_nodeId);
            }
        );
        
        scene->onNodeRemoved.subscribe(
            [this](uint32_t t_nodeId) {
                if(m_nodeItems.find(t_nodeId) == m_nodeItems.end()) return;
                m_nodeItems.erase(t_nodeId);
                m_drawOrder.erase(std::remove(m_drawOrder.begin(), m_drawOrder.end(), t_nodeId), m_drawOrder.end());
            }
        );

        scene->onConnectionAdded.subscribe(
            [this](const uint32_t t_sourceId, const uint32_t t_sourceIndex, const uint32_t t_destId, const uint32_t t_destIndex) {
                std::shared_ptr<ConnectionItem> conn = std::make_shared<ConnectionItem>();
                conn->setSource(m_nodeItems[t_sourceId], t_sourceIndex);
                conn->setDestination(m_nodeItems[t_destId], t_destIndex);
                m_nodeConnections.push_back(conn);
            }
        );

        scene->onConnectionRemoved.subscribe(
            [this](const uint32_t t_sourceId, const uint32_t t_destId, const uint32_t t_destIndex) {
                m_nodeConnections.erase(std::remove_if(m_nodeConnections.begin(), m_nodeConnections.end(), 
                    [&](const std::shared_ptr<ConnectionItem>& conn){
                        auto sourceNodeItem = conn->sourceNode();
                        auto destNodeItem = conn->destinationNode();
                        return sourceNodeItem && destNodeItem && sourceNodeItem == m_nodeItems[t_sourceId] && destNodeItem == m_nodeItems[t_destId] && conn->destinationIndex() == t_destIndex;
                    }),
                m_nodeConnections.end());
            }
        );
    }
}

void NodeGraph::draw()
{
    beginTab("Node Graph", m_padding);
    createNodeMenu();

    handleInputs();
    
    drawConnections();
    drawNodes();
    endTab();
}

void NodeGraph::handleInputs()
{
    handleCreateNode();

    handleNodeInteractions();

    handleDragGraph();
    handleKeyInput();
}

/**
 * @brief handle if user right clicked or pressed Tab to create a Node
 * 
 */
void NodeGraph::handleCreateNode() {
    ImVec2 regionAvail = ImGui::GetContentRegionAvail();
	ImVec2 region = ImVec2(std::max(regionAvail.x, 100.0f), std::max(regionAvail.y, 100.0f));
    ImGui::InvisibleButton("nodegraph_click_area", region, ImGuiButtonFlags_None);
    if(ImGui::IsItemHovered() && (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || ImGui::IsKeyPressed(ImGuiKey_Tab)) ) {
        ImGui::OpenPopup("node_menu");
        m_focusSearchBar = true;
    }
}

void NodeGraph::createNodeMenu() {
	ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(70, 70, 70, 255));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));

	if (ImGui::BeginPopup("node_menu")) {
		const auto& menuItems = NodesInfo::getMenuItems();

        searchBar();

        for (auto& [menuName, items] : menuItems) {
            if (m_searchNode.empty()) {
                if (ImGui::BeginMenu(menuName.c_str())) {
                    drawNodesItems(items);
                    ImGui::EndMenu();
                }
            } else {
                for (const auto& item: items) {
                    if (toLower(item.name).find(toLower(m_searchNode)) == std::string::npos) continue;
                    if (ImGui::MenuItem(item.name.c_str(), nullptr)) {
                        if (auto scene = m_scene.lock()) {
                            scene->addNode(item.createNode());
                        }
                    }
                }
            }
        }
		ImGui::EndPopup();
	} else {
        m_searchNode.clear();
    }

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}

void NodeGraph::searchBar() {
    ImGui::PushStyleColor(ImGuiCol_FrameBg, m_searchBarColor);

    if (m_focusSearchBar) {
        ImGui::SetKeyboardFocusHere();
        m_focusSearchBar = false;
    }

    if (ImGui::InputTextWithHint("##searchBar", "Search", (char*)m_searchNode.c_str(), m_searchNode.capacity() + 1, ImGuiInputTextFlags_CallbackResize, StringImGuiCallBack, (void*)&m_searchNode)) {
        
    }
    ImGui::PopStyleColor();
}

void NodeGraph::drawNodesItems(const std::vector<NodeMenuItem>& items) {

	for (size_t i = 0; i < items.size(); ++i) {
		const auto& item = items[i];
		if (ImGui::MenuItem(item.name.c_str(), nullptr)) {
            if (auto scene = m_scene.lock()) {
                scene->addNode(item.createNode());
            }
		}
		if (i < items.size() - 1) {
			ImGui::Separator();
		}
	}
}

/**
 * @brief Handle all mouse buttons when the user hovered a NodeItem
 * 
 * 
 */
void NodeGraph::handleNodeInteractions() {

    bool isWindowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && isWindowHovered) {
        m_isLeftClicked = true;
        refreshHoveredNode();
    }
    
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        leftMouseReleased();
        m_isLeftClicked = false;
    }

    if (m_isLeftClicked) {
        leftMouseDown();
    }

    if (ImGui::IsKeyReleased(ImGuiKey_R) && isWindowHovered) {
        renderSelectedNode();
    }
}

void NodeGraph::leftMouseReleased() {
    if (m_isNodeDrag) {
        m_isNodeDrag = false;
        return;
    }

    handleNodeClicked();   
}

void NodeGraph::handleNodeClicked() {

    IOClickedInfos ioInfos = isHoveredIO();
    if (ioInfos.nodeItem) {
        addConnection(ioInfos);
        return;
    }

    clearCurrentConnection();

    if (!m_nodeHovered) {
        clearSelection();
        return;
    }

    if(ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
        addToSelection(m_nodeHovered);
    } else {
        clearSelection();
        addToSelection(m_nodeHovered);
        onNodeSelected.notify(m_nodeHovered->node());
    }
}

void NodeGraph::leftMouseDown() { 
    if (!m_nodeHovered) return;
    
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 dragDelta = io.MousePos - io.MousePosPrev;

    if (!m_isNodeDrag) {
        if (std::abs(dragDelta.x) > 0.01 || std::abs(dragDelta.y) > 0.01) {
            m_isNodeDrag = true;
        }
    }

    if (!m_nodeHovered->isSelected()) {
        m_nodeHovered->moveBy(dragDelta);
        return;
    }

    for (auto node: m_selectedNodes){
        node->moveBy(dragDelta);
    }
    
}

void NodeGraph::refreshHoveredNode() {
    m_nodeHovered = nullptr;
    for (auto it = m_drawOrder.rbegin(); it != m_drawOrder.rend(); ++it) {
        auto nodeItem = m_nodeItems[*it];
        if (nodeItem->isHovered()) {
            m_nodeHovered = nodeItem;
            return;
        }
    }
}

void NodeGraph::renderSelectedNode() {
    if(m_selectedNodes.size() == 0) return;
    if (auto scene = m_scene.lock()){
        auto nodeItem = m_selectedNodes.begin()->get();
        scene->cookNode(nodeItem->node());
    }
}

IOClickedInfos NodeGraph::isHoveredIO() {
    for(auto [id, nodeItem]: m_nodeItems) {
        int inputIndex = nodeItem->inputIOHovered();
        if (inputIndex >= 0) {
            return IOClickedInfos{nodeItem, IOType::INPUT, inputIndex};
        }  

        int outputIndex = nodeItem->outputIOHovered();
        if (outputIndex >= 0) {
            return IOClickedInfos{nodeItem, IOType::OUTPUT, outputIndex};
        } 
    }

    return IOClickedInfos{};
}

void NodeGraph::addConnection(const IOClickedInfos& t_infos) {
    if(!m_currentConnection) {
        std::unique_ptr<ConnectionItem> conn = std::make_unique<ConnectionItem>();
        if(t_infos.type == IOType::INPUT) {
            conn->setDestination(t_infos.nodeItem, t_infos.index);
        } else {
            conn->setSource(t_infos.nodeItem, t_infos.index);
        }
        m_currentConnection = std::move(conn);
        return;
    }

    // Same IO type clicked
    IOType currentType = m_currentConnection->sourceNode() ? IOType::OUTPUT: IOType::INPUT;
    if (currentType == t_infos.type){
        clearCurrentConnection();
        addConnection(t_infos);
        return;
    }
    
    std::shared_ptr<NodeItem> currentNode = currentType == IOType::OUTPUT ? m_currentConnection->sourceNode() : m_currentConnection->destinationNode();
    uint32_t currentIndex = currentType == IOType::OUTPUT ? m_currentConnection->sourceIndex() : m_currentConnection->destinationIndex();
    
    // Create connection
    if (currentType == IOType::INPUT) {
        currentNode->node()->setInput(currentIndex, t_infos.nodeItem->node(), t_infos.index);
    } else {
        t_infos.nodeItem->node()->setInput(t_infos.index, currentNode->node(), currentIndex);
    }
    clearCurrentConnection();
}

void NodeGraph::clearCurrentConnection() {
    m_currentConnection = nullptr;
}

/**
 * @brief Move all the graph when user press middle click
 * 
 */
void NodeGraph::handleDragGraph() {
    ImGuiIO& io = ImGui::GetIO();
    bool isWindowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

    if (m_isGraphDrag) {
        ImVec2 dragDelta = io.MousePos - io.MousePosPrev;
        for (auto& [id, nodeItem]: m_nodeItems) {
            nodeItem->moveBy(dragDelta);
        }
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle) && isWindowHovered) {
        m_isGraphDrag = true;
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle)) {
        m_isGraphDrag = false;
    }
}

void NodeGraph::handleKeyInput() {

    bool isWindowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
    if (!isWindowHovered) return;
    
    if (ImGui::IsKeyPressed(ImGuiKey_Delete)) {
        removeSelectedNodes();
    }

    if (ImGui::IsKeyDown(ImGuiKey_Y)) {
        addCuttingLine();
        drawCuttingLines();
        checkCutConnection();
    } else if (ImGui::IsKeyReleased(ImGuiKey_Y)){
        m_cuttingsLines.clear();
    }

}

void NodeGraph::addCuttingLine() {
    ImGuiIO& io = ImGui::GetIO();
    m_cuttingsLines.push(CuttingLine{
        io.MousePosPrev, io.MousePos
    });
}

void NodeGraph::drawCuttingLines()
{
    for (const auto& line: m_cuttingsLines) {
        line.draw();
    }
}

void NodeGraph::checkCutConnection() {
    ImGuiIO& io = ImGui::GetIO();
    if (io.MousePos == io.MousePosPrev) return;

    for (const auto& connection : m_nodeConnections) {
        if (lineIntersect(connection->sourcePosition(), connection->destinationPosition(), io.MousePos, io.MousePosPrev)) {
            connection->deleteConnection();
            break;
        }
    }
}

void NodeGraph::removeSelectedNodes() {
    if (auto scene = m_scene.lock()) {
        for (auto nodeItem: m_selectedNodes) {
            if (nodeItem == nullptr) continue;

            if (auto node = nodeItem->node()) {
                scene->removeNode(node->name());
            }
        }
    }

    m_selectedNodes.clear();
}

void NodeGraph::drawConnections() {
    for (auto conn: m_nodeConnections){
        conn->draw();
    }

    if (m_currentConnection) 
        m_currentConnection->draw();
} 

/**
 * @brief Draw all nodes and stores the top hovered node in `m_nodeHovered`
 * 
 */
void NodeGraph::drawNodes()
{
    bool isWindowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
    for (uint32_t id: m_drawOrder) {
        m_nodeItems[id]->draw();
    }
}

void NodeGraph::clearSelection()
{
    for (auto node: m_selectedNodes) {
        node->setSelected(false);
    }
    m_selectedNodes.clear();
    onNodeSelected.notify(std::weak_ptr<Node>());
}

/**
 * @brief Add node to selection, can be removed if it is already selected and `t_removeIfAlreadySelected` is set to `true`
 * 
 * @param t_nodeItem The Node Item to select
 * @param t_removeIfAlreadySelected Remove the item from selection if it was already selected
 * 
 */
void NodeGraph::addToSelection(const std::shared_ptr<NodeItem>& t_nodeItem, const bool t_removeIfAlreadySelected) {
    if (t_removeIfAlreadySelected) {
        if (m_selectedNodes.find(t_nodeItem) != m_selectedNodes.end()) { // Remove Node Item if its already selected
            m_selectedNodes.erase(t_nodeItem);
            t_nodeItem->setSelected(false);
            return;
        }
    }

    m_selectedNodes.insert(t_nodeItem);
    t_nodeItem->setSelected(true);
}

}