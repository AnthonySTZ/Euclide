#pragma once

#include "node_graph.h"

namespace butter {

class NodeGraphMenu {
  public:
    NodeGraphMenu(const std::weak_ptr<NodeGraph> t_nodeGraph) : m_graph(std::move(t_nodeGraph)){};
    ~NodeGraphMenu() = default;

    void render();

  private:
    void beginStyle() const;
    void endStyle() const;

    std::weak_ptr<NodeGraph> m_graph;
};

} // namespace butter