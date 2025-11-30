#include "node_graph_renderer.h"

namespace butter {

void NodeGraphRenderer::render() const {
    drawConnections();
    drawNodes();
}

void NodeGraphRenderer::drawConnections() const {
    for (ConnectionItem& conn : m_graph.connections) {
        conn.draw();
    }
}

void NodeGraphRenderer::drawNodes() const {
    for (const uint32_t id : m_graph.drawNodesOrder) {
        m_graph.nodes[id]->draw();
    }
}

} // namespace butter
