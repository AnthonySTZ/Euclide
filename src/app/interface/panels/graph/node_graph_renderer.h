#pragma once

#include "node_graph.h"

namespace butter {

class NodeGraphRenderer {
  public:
    NodeGraphRenderer(const std::weak_ptr<NodeGraph> t_nodeGraph) : m_graph(std::move(t_nodeGraph)){};

    void render() const;
    std::shared_ptr<NodeItem> getNodeAt(ImVec2 t_mousePosition) const;

  private:
    void drawNodes() const;
    void drawConnections() const;

  private:
    std::weak_ptr<NodeGraph> m_graph;
};

} // namespace butter