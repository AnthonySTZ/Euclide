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
    void handleKeyInputs();

    [[nodiscard]] bool isDragging() const;

    void handleDragging() const;
    void handleLeftMouseRelease() const;

    void dragNodes(const ImVec2& t_dragDelta) const;

  private:
    std::weak_ptr<NodeGraph> m_graph;
    NodeGraphRenderer* m_graphRenderer = nullptr;

    bool m_isWindowHovered = false;

    bool m_mouseButtonLeftDown = false;
    bool m_isMouseDrag = false;
    std::optional<uint32_t> m_draggingNode = std::nullopt;
    std::optional<IOInfos> m_ioClicked = std::nullopt;

    static constexpr float DRAG_THRESHOLD = 0.01f;
};

} // namespace butter