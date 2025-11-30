#pragma once

#include "nodes/node.h"

#include "interface/utils/imgui_utils.h"
#include <memory>

namespace butter {

/// @brief Represents a visual node in the node editor UI.
class NodeItem {
  public:
    /// @brief Construct a NodeItem with the associated Node and its initial position.
    /// @param t_node Shared pointer to the underlying Node data.
    /// @param t_position Initial position of the node in UI coordinates.
    NodeItem(const std::shared_ptr<Node>& t_node, const ImVec2 t_position);

    /// @brief Draw the node and its IO slots in the current ImGui window.
    void draw();

    /// @brief Check if the node rectangle is currently hovered by the mouse.
    /// @return True if hovered, false otherwise.
    [[nodiscard]] bool isHovered() const;

    /// @brief Determine if any input slot is hovered by the mouse.
    /// @return Index of hovered input slot, or -1 if none.
    [[nodiscard]] int inputIOHovered() const;

    /// @brief Determine if any output slot is hovered by the mouse.
    /// @return Index of hovered output slot, or -1 if none.
    [[nodiscard]] int outputIOHovered() const;

    /// @brief Move the node by a delta in UI coordinates.
    /// @param t_delta Offset to apply to the node's position.
    void moveBy(const ImVec2& t_delta);

    /// @brief Set the selection state of the node.
    /// @param t_isSelected True if the node is selected.
    void setSelected(const bool t_isSelected) { m_isSelected = t_isSelected; }

    /// @brief Query if the node is currently selected.
    /// @return True if selected, false otherwise.
    [[nodiscard]] bool isSelected() const noexcept { return m_isSelected; }

    /// @brief Get the position of a specific input slot.
    /// @param index Index of the input slot.
    /// @return Position of the input slot in UI coordinates.
    [[nodiscard]] ImVec2 getInputIOPosition(uint32_t index) const;

    /// @brief Get the position of a specific output slot.
    /// @param index Index of the output slot.
    /// @return Position of the output slot in UI coordinates.
    [[nodiscard]] ImVec2 getOutputIOPosition(uint32_t index) const;

    /// @brief Get the underlying Node shared pointer.
    /// @return Shared pointer to the Node, may be nullptr if expired.
    [[nodiscard]] std::shared_ptr<Node> node() const { return m_node.lock(); }

  private:
    /// @brief Draw the node rectangle with proper colors.
    /// @param t_nodeName Name of the node.
    /// @param isRender If true, render with the active color, otherwise a default background.
    void drawRect(const std::string& t_nodeName, const bool isRender);

    /// @brief Draw the input and output slots.
    /// @param t_numberOfInputs Number of input slots.
    /// @param t_numberOfOutputs Number of output slots.
    void drawIOs(const int t_numberOfInputs, const int t_numberOfOutputs);

    /// @brief Determine which IO slot is hovered based on positions.
    /// @param t_ioPositions Vector of IO positions to test against the mouse.
    /// @return Index of hovered slot, or -1 if none.
    [[nodiscard]] int isIOsHovered(const std::vector<ImVec2>& t_ioPositions) const;

    /// @brief Compute positions of IO slots along a line.
    /// @param t_numberOfIOs Number of slots to place.
    /// @param t_linePosition Starting position of the line.
    /// @return Vector of computed slot positions.
    std::vector<ImVec2> drawIOsOnLine(const int t_numberOfIOs, ImVec2 t_linePosition) const;

  private:
    std::weak_ptr<Node> m_node; ///< Associated data node.
    bool m_isSelected = false;  ///< Selection state.

    ImVec2 m_position;                       ///< Node position in UI coordinates.
    std::vector<ImVec2> m_inputIOPositions;  ///< Cached input slot positions.
    std::vector<ImVec2> m_outputIOPositions; ///< Cached output slot positions.

    ImVec2 m_size = ImVec2(100, 30);                             ///< Node rectangle size.
    ImU32 m_color = IM_COL32(100, 100, 100, 255);                ///< Base node color.
    ImU32 m_renderColor = IM_COL32(146, 63, 80, 255);            ///< Active render color.
    ImU32 m_outlineColor = IM_COL32(200, 200, 200, 255);         ///< Rectangle outline color.
    ImU32 m_selectedOutlineColor = IM_COL32(255, 178, 102, 255); ///< Outline color when selected.

    static constexpr float RADIUS = 5.5f;                                   ///< Corner rounding radius.
    static constexpr float RADIUS_2 = RADIUS * RADIUS;                      ///< Precomputed squared radius.
    static constexpr float SPACING = 3.0f;                                  ///< Spacing between slots.
    static constexpr ImU32 IO_OUTLINE_COLOR = IM_COL32(200, 200, 200, 255); ///< IO slot outline color.
    static constexpr ImU32 IO_COLOR = IM_COL32(170, 170, 170, 255);         ///< IO slot color.
};

} // namespace butter