#pragma once

#include "node_graph_renderer.h"
#include "interface/panels/graph/node/node_item_interaction.h"

namespace euclide {

/// @brief Handles user input for a NodeGraph, including mouse clicks, drags, and keyboard shortcuts.
/// @details This class does not perform rendering itself; it queries the NodeGraph state and interacts
/// with NodeGraphRenderer and NodeGraphInteraction to update node positions, selection, connections, and
/// context menu interactions.
class NodeGraphInputHandler {
  public:
    /// @brief Constructs a NodeGraphInputHandler.
    /// @param t_nodeGraph Weak pointer to the NodeGraph this input handler will operate on.
    /// @param t_graphRenderer Pointer to the NodeGraphRenderer to notify for visual feedback (e.g., connections,
    /// cutting lines).
    NodeGraphInputHandler(const std::weak_ptr<NodeGraph> t_nodeGraph, NodeGraphRenderer* t_graphRenderer)
        : m_graph(std::move(t_nodeGraph)), m_graphRenderer(t_graphRenderer){};

    ~NodeGraphInputHandler() = default;

    /// @brief Polls all input events for this frame.
    /// @details Should be called once per frame to handle mouse and keyboard input,
    /// update node dragging, selection, and context menu interactions.
    void pollEvents();

  private:
    /// @brief Handles right-click or key events to open the node graph context menu.
    void handleContextMenu();

    /// @brief Handles all mouse input events including clicks, releases, and dragging.
    void handleMouseInputs();

    void repositionCurrentConnection();

    /// @brief Handle Left Mouse clicked, if its on a node, io, ...
    void handleLeftMouseClicked();

    /// @brief Handles keyboard input events related to node operations (e.g., cut connections, render selected node).
    void handleKeyInputs();

    /// @brief Determines whether the user is moving its cursor.
    /// @return True if dragging exceeds the threshold, false otherwise.
    [[nodiscard]] bool isDragging() const;

    /// @brief Updates node positions while dragging.
    void handleDragging() const;

    void startConnection(const IOInfos& t_infos);

    void endConnection();

    /// @brief Handles logic when the left mouse button is released.
    /// @details Handles connection creation, node selection, and clearing selections.
    void handleLeftMouseRelease() const;

    /// @brief Moves selected or dragged nodes by the given delta.
    /// @param t_dragDelta The movement vector to apply to nodes.
    void dragNodes(const ImVec2& t_dragDelta) const;

    /// @brief Reset all mouse variables when mouse is released.
    void resetMouseInteraction();

  private:
    std::weak_ptr<NodeGraph> m_graph;             ///< Weak pointer to the NodeGraph this handler operates on.
    NodeGraphRenderer* m_graphRenderer = nullptr; ///< Pointer to the renderer

    bool m_isWindowHovered = false; ///< True if the node graph window is hovered by the mouse.

    bool m_mouseButtonLeftDown = false;                    ///< True if the left mouse button is currently held.
    bool m_isMouseDrag = false;                            ///< True if the user is dragging nodes.
    bool m_isBoxSelecting = false;                         ///< True if the user is box selecting.
    ImVec2 m_boxStart{};                                   ///< Start position of the box selection.
    std::optional<uint32_t> m_draggingNode = std::nullopt; ///< ID of the node currently being dragged, if any.
    std::optional<IOInfos> m_ioClicked = std::nullopt;     ///< Information about an IO slot clicked for connections.
    std::unique_ptr<ConnectionItem> m_currentConnection;   ///< Currently active connection being dragged.

    static constexpr float DRAG_THRESHOLD = 0.01f;        ///< Minimum drag distance to initiate dragging.
    static constexpr float BIG_IO_RADIUS = 16.0f * 16.0f; ///< Minimum drag distance to initiate dragging.
};

} // namespace euclide
