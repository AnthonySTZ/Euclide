#pragma once

#include "panel.h"

#include "node_graph.h"

namespace butter {

/// @brief Panel displaying the parameters of the currently selected node.
///
/// Subscribes to node selection events in a NodeGraph and dynamically shows
/// the fields of the selected node for editing.
class Parameters : public Panel {
  public:
    /// @brief Construct a Parameters panel attached to a NodeGraph.
    /// @param t_nodeGraph Weak pointer to the NodeGraph to observe node selection.
    Parameters(const std::shared_ptr<NodeGraph>& t_nodeGraph);

    ~Parameters() = default;

    /// @brief Draw the parameters panel. Implements Panel::draw().
    void draw() override;

  private:
    /// @brief Draw all fields of the currently selected node.
    void drawParameters();

    /// @brief Draw the name of the selected node at the top of the panel.
    /// @param t_title The name of the node.
    void drawTitleName(const std::string& t_title);

    /// @brief Push the ImGui style settings used for the panel fields.
    void pushStyle();

    /// @brief Pop the ImGui style settings used for the panel fields.
    void popStyle();

  private:
    std::weak_ptr<NodeGraph> m_nodeGraph; ///< Observed NodeGraph for node selection.
    std::weak_ptr<Node> m_node;           ///< Currently selected node.

    static constexpr float TITLE_SPACING = 20.0f; ///< Vertical spacing after node title.
};

} // namespace butter