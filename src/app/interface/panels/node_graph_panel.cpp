#include "node_graph_panel.h"

namespace butter {

NodeGraph::NodeGraph(const std::shared_ptr<Scene>& t_scene)
    : m_scene(t_scene)
{
    if (auto scene = m_scene.lock()) {
        scene->onNodeAdded.subscribe(
            [this](uint32_t t_nodeId, std::shared_ptr<Node> t_node) {
                m_nodeItems.try_emplace(t_nodeId, std::make_shared<NodeItem>(t_node));
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

    endTab();
}

}