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
            }
        );
    
        scene->onNodeRemoved.subscribe(
            [this](uint32_t t_nodeId) {
                if(m_nodeItems.find(t_nodeId) == m_nodeItems.end()) return;
                m_nodeItems.erase(t_nodeId);
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
    handleNodeDragging();
    handleDragGraph();
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
 * @brief Update dragging node position if needed
 * 
 */
void NodeGraph::handleNodeDragging() {
    ImGuiIO& io = ImGui::GetIO();

    if (m_isDrag) {
        ImVec2 dragDelta = io.MousePos - io.MousePosPrev;
        m_nodeHovered->moveBy(dragDelta);
        if (std::abs(dragDelta.x) > 0.01 || std::abs(dragDelta.y) > 0.01) {
            m_isClicked = false;
        }   
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        if (m_nodeHovered) {
            m_isDrag = true;
            m_isClicked = true;
        } else {
            m_selectedNode = nullptr;
        }
    } 

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        m_isDrag = false;
        if (m_isClicked) {
            m_selectedNode = m_nodeHovered;
            m_isClicked = false;
        }
    }
}

/**
 * @brief Move all the graph when user press middle click
 * 
 */
void NodeGraph::handleDragGraph() {
    ImGuiIO& io = ImGui::GetIO();

    if (m_isGraphDrag) {
        ImVec2 dragDelta = io.MousePos - io.MousePosPrev;
        for (auto& [id, nodeItem]: m_nodeItems) {
            nodeItem->moveBy(dragDelta);
        }
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle)) {
        m_isGraphDrag = true;
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle)) {
        m_isGraphDrag = false;
    }
}

/**
 * @brief Draw all nodes and stores the top hovered node in `m_nodeHovered`
 * 
 */
void NodeGraph::drawNodes()
{
    if (!m_isDrag)
        m_nodeHovered = nullptr;

    bool isWindowHovered = ImGui::IsWindowHovered();
    for (auto& [id, nodeItem]: m_nodeItems) {
        nodeItem->draw(nodeItem == m_selectedNode);

        if(m_isDrag || !isWindowHovered) continue;
        if (nodeItem->isHovered()) {
            m_nodeHovered = nodeItem;
        }
    }
}

}