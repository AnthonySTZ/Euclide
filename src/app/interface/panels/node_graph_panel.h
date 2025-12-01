#pragma once

#include "interface/panels/panel.h"

#include "graph/node_graph_renderer.h"
#include "graph/node_graph_input_handler.h"
#include "graph/node_graph_menu.h"

namespace euclide {

/// @brief Panel for displaying and interacting with a NodeGraph.
/// @details Combines the NodeGraph, renderer, input handler, and menu into a single UI panel.
class NodeGraphPanel : public Panel {
  public:
    /// @brief Constructs a NodeGraphPanel with a given Scene.
    /// @param t_scene Shared pointer to the Scene to bind to the NodeGraph.
    NodeGraphPanel(const std::shared_ptr<Scene> t_scene);

    ~NodeGraphPanel() override = default;

    /// @brief Draws the panel.
    /// @details Polls input events, renders the context menu, and renders the NodeGraph.
    void draw() override;

    /// @brief Access the underlying NodeGraph.
    /// @return Weak pointer to the NodeGraph.
    [[nodiscard]] std::weak_ptr<NodeGraph> graph() { return m_graph; }

  private:
    std::shared_ptr<NodeGraph> m_graph;        ///< Shared pointer to the NodeGraph.
    NodeGraphRenderer m_graphRenderer;         ///< Renderer responsible for drawing nodes and connections.
    NodeGraphInputHandler m_graphInputHandler; ///< Handles user input on the graph.
    NodeGraphMenu m_graphMenu;                 ///< Context menu for adding nodes.
};

} // namespace euclide
