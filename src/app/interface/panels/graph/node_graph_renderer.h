#pragma once

#include "node_graph.h"

namespace butter {

class NodeGraphRenderer {
  public:
    NodeGraphRenderer(NodeGraph& t_nodeGraph) : m_graph(t_nodeGraph){};

    void render() const;

  private:
    void drawNodes() const;
    void drawConnections() const;

  private:
    NodeGraph& m_graph;
};

} // namespace butter