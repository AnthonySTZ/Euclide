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
    ImGuiIO& io = ImGui::GetIO();

    ImVec2 regionAvail = ImGui::GetContentRegionAvail();
	ImVec2 region = ImVec2(std::max(regionAvail.x, 100.0f), std::max(regionAvail.y, 100.0f));
    ImGui::InvisibleButton("nodegraph_click_area", region, ImGuiButtonFlags_None);
    if(ImGui::IsItemHovered() && (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || ImGui::IsKeyPressed(ImGuiKey_Tab)) ) {
        if (auto scene = m_scene.lock()) {
            scene->addNode(std::make_shared<Cube>());
        }
    }

    if (m_isDrag) {
        ImVec2 dragDelta = io.MousePos - io.MousePosPrev;
        m_nodeHovered->moveBy(dragDelta);
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        if (m_nodeHovered != nullptr) {
            m_isDrag = true;
        }
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        m_isDrag = false;
    }
}

void NodeGraph::drawNodes()
{
    if (!m_isDrag)
        m_nodeHovered = nullptr;

    for (auto& [id, nodeItem]: m_nodeItems) {
        nodeItem->draw();

        if (!m_isDrag) {
            if (nodeItem->isHovered() && ImGui::IsWindowHovered()) {
                m_nodeHovered = nodeItem;
            }
        }
    }
}

}