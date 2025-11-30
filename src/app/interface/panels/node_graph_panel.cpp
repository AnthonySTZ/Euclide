#include "node_graph_panel.h"

void butter::NodeGraphPanel::draw() {
    beginTab("Node Graph", m_padding);

    m_graphRenderer.render();

    endTab();
}