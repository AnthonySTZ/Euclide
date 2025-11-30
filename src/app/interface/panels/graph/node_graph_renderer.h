#pragma once

#include "node_graph.h"

namespace butter {

class NodeGraphRenderer {
  public:
    NodeGraphRenderer(const std::shared_ptr<NodeGraph>& t_nodeGraph) : m_graph(t_nodeGraph){};

    void render() const;

  private:
    void drawNodes() const;
    void drawConnections() const;

  private:
    std::shared_ptr<NodeGraph> m_graph;
};

} // namespace butter