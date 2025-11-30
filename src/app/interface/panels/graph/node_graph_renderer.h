#pragma once

#include "node_graph.h"

namespace butter {

class NodeGraphRenderer {
  public:
    NodeGraphRenderer(const std::weak_ptr<NodeGraph> t_nodeGraph) : m_graph(std::move(t_nodeGraph)){};

    void render() const;

  private:
    void drawNodes() const;
    void drawConnections() const;

  private:
    std::weak_ptr<NodeGraph> m_graph;
};

} // namespace butter