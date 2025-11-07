#include "node_graph_panel.h"

#include "nodes/geometry/cube.h"

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
    }
}

void NodeGraph::draw()
{
    beginTab("Node Graph", m_padding);

    handleInputs();
    
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
        if (auto scene = m_scene.lock()) {
            scene->addNode(std::make_shared<Cube>());
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
    if(!isWindowHovered) return;

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        refreshHoveredNode();
    }

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        leftMouseDown();
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        leftMouseReleased();
    }

}

void NodeGraph::leftMouseReleased() {
    if (!m_nodeHovered) {
        clearSelection();
        return;
    }

    if (m_isNodeDrag) {
        m_isNodeDrag = false;
        return;
    }

    if(ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
        addToSelection(m_nodeHovered);
    } else {
        clearSelection();
        addToSelection(m_nodeHovered);
    }
}

void NodeGraph::leftMouseDown() {
    if (!m_nodeHovered) return;
    
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 dragDelta = io.MousePos - io.MousePosPrev;
    if (m_isNodeDrag) {
        m_nodeHovered->moveBy(dragDelta);
        return;
    }

    if (std::abs(dragDelta.x) > 0.01 || std::abs(dragDelta.y) > 0.01) {
        m_isNodeDrag = true;
    }
    
}

void NodeGraph::refreshHoveredNode() {
    m_nodeHovered = nullptr;
    for (int i = m_drawOrder.size() - 1; i >= 0; --i) {
        uint32_t id = m_drawOrder[i];
        auto nodeItem = m_nodeItems[id]; 
        if (nodeItem->isHovered()) {
            m_nodeHovered = nodeItem;
            return;
        }
    }
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
    
    if (ImGui::IsKeyPressed(ImGuiKey_Delete) && isWindowHovered) {
        removeSelectedNodes();
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