#include "parameters.h"

namespace butter {

Parameters::Parameters(const std::shared_ptr<NodeGraph> &t_nodeGraph)
    : m_nodeGraph(t_nodeGraph)
{
    if (auto nodeGraph = m_nodeGraph.lock()){
        nodeGraph->onNodeSelected.subscribe([this](std::weak_ptr<Node> t_node) {
            m_node = t_node;
        });
    }
}

void Parameters::draw()
{
    beginTab("Parameters", m_padding);
    drawParameters();
    endTab();
}

void Parameters::drawParameters() {
    if (auto node = m_node.lock()) {
        ImGui::Text(node->name().c_str());
    }
}

}

