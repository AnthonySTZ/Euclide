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

    [[nodiscard]] bool isDragging() const;

    void handleDragging() const;

  private:
    std::weak_ptr<NodeGraph> m_graph;
    NodeGraphRenderer* m_graphRenderer = nullptr;

    bool m_mouseButtonLeftDown = false;
    bool m_isMouseDrag = false;
    std::weak_ptr<NodeItem> m_draggingNode = std::weak_ptr<NodeItem>();

    static constexpr float DRAG_THRESHOLD = 0.01f;
};

} // namespace butter