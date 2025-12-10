#pragma once

#include "nodes/node.h"

#include "interface/utils/imgui_utils.h"
#include <memory>

namespace euclide {

class NodeItemRenderer;
class NodeItemInteraction;

/// @brief Represents a visual node in the node editor UI.
class NodeItemModel {
  public:
    /// @brief Construct a NodeItemModel with the associated Node and its initial position.
    /// @param t_node Shared pointer to the underlying Node data.
    /// @param t_position Initial position of the node in UI coordinates.
    NodeItemModel(const std::weak_ptr<Node> t_node, const ImVec2 t_position) : m_node(t_node), m_position(t_position) {}

    /// @brief Move the node by a delta in UI coordinates.
    /// @param t_delta Offset to apply to the node's position.
    inline void moveBy(const ImVec2& t_delta) { m_position += t_delta; }

    /// @brief Set the selection state of the node.
    /// @param t_isSelected True if the node is selected.
    inline void setSelected(const bool t_isSelected) { m_isSelected = t_isSelected; }

    /// @brief Query if the node is currently selected.
    /// @return True if selected, false otherwise.
    [[nodiscard]] inline bool isSelected() const noexcept { return m_isSelected; }

    /// @brief Get the underlying Node shared pointer.
    /// @return Shared pointer to the Node, may be nullptr if expired.
    [[nodiscard]] inline std::shared_ptr<Node> node() const { return m_node.lock(); }

    /// @brief Get the node position
    /// @return Position of the node in UI coordinates.
    [[nodiscard]] inline ImVec2 position() const noexcept { return m_position; }

    /// @brief Get the node size
    /// @return Size of the node in UI coordinates.
    [[nodiscard]] inline ImVec2 size() const noexcept { return m_size; }

  private:
    std::weak_ptr<Node> m_node; ///< Associated data node.
    bool m_isSelected = false;  ///< Selection state.

    ImVec2 m_position; ///< Node position in UI coordinates.

    ImVec2 m_size = ImVec2(100, 30);                             ///< Node rectangle size.
    ImU32 m_color = IM_COL32(100, 100, 100, 255);                ///< Base node color.
    ImU32 m_renderColor = IM_COL32(146, 63, 80, 255);            ///< Active render color.
    ImU32 m_outlineColor = IM_COL32(200, 200, 200, 255);         ///< Rectangle outline color.
    ImU32 m_selectedOutlineColor = IM_COL32(255, 178, 102, 255); ///< Outline color when selected..

    friend NodeItemRenderer;
    friend NodeItemInteraction;
};

} // namespace euclide