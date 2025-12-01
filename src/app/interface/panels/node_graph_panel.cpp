#include "node_graph_panel.h"

namespace euclide {

NodeGraphPanel::NodeGraphPanel(const std::shared_ptr<Scene> t_scene)
    : m_graph(std::make_shared<NodeGraph>(std::move(t_scene))), m_graphRenderer(m_graph),
      m_graphInputHandler(m_graph, &m_graphRenderer), m_graphMenu(m_graph) {
}

void NodeGraphPanel::draw() {
    beginTab("Node Graph", m_padding);

    m_graphInputHandler.pollEvents();
    m_graphMenu.render();
    m_graphRenderer.render();

    endTab();
}

} // namespace euclide