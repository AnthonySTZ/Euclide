#include "parameters.h"

#include "interface/panels/fields/field_drawer.h"

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
    FieldDrawer drawer;
    if (auto node = m_node.lock()) {
        for(const auto& [name, field]: node->fields()) {
            field->accept(name, drawer);
        }
    }
}

}

