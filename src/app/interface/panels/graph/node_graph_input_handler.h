#pragma once

#include "node_graph_renderer.h"

namespace butter {

class NodeGraphInputHandler {
  public:
    NodeGraphInputHandler(const std::weak_ptr<NodeGraph> t_nodeGraph, NodeGraphRenderer* t_graphRenderer)
        : m_graph(std::move(t_nodeGraph)), m_graphRenderer(t_graphRenderer){};
    ~NodeGraphInputHandler() = default;

    void pollEvents();

  private:
    void handleContextMenu();
    void handleMouseInputs();

  private:
    std::weak_ptr<NodeGraph> m_graph;
    NodeGraphRenderer* m_graphRenderer = nullptr;

    bool m_mouseButtonLeftDown = false;
};

} // namespace butter