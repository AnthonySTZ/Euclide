#pragma once

#include "interface/utils/imgui_utils.h"

namespace butter {

/// @brief Represents a simple line to be drawn in the ImGui context.
class CuttingLine {

  public:
    /// @brief Construct a cutting line from start to end points.
    /// @param t_start Starting position of the line.
    /// @param t_end Ending position of the line.
    CuttingLine(const ImVec2& t_start, const ImVec2& t_end) : m_start{t_start}, m_end{t_end} {}

    /// @brief Draw the line in the current ImGui window.
    /// @note This function assumes that an ImGui window is active.
    void draw() const noexcept;

  private:
    ImVec2 m_start; ///< Start point of the line
    ImVec2 m_end;   ///< End point of the line

    static constexpr ImU32 COLOR = IM_COL32(255, 50, 50, 255); ///< Line color
    static constexpr float THICKNESS = 1.5f;                   ///< Line thickness
};

} // namespace butter
