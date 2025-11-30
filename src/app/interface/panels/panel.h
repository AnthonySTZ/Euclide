#pragma once

#include "interface/utils/imgui_utils.h"

namespace butter {

/// @brief Abstract base class for a UI panel using ImGui.
///
/// Provides helper functions to create tabbed panels with a title bar and padding.
/// Derived classes must implement the draw() method.
class Panel {
  public:
    Panel() = default;
    virtual ~Panel() = default;

    /// @brief Draw the panel content.
    ///
    /// Must be implemented by derived classes to define panel behavior.
    virtual void draw() = 0;

  protected:
    /// @brief Begin a panel tab with a title, padding, and background color.
    /// @param t_name Name of the tab (displayed in the title bar).
    /// @param t_padding Padding inside the tab window.
    /// @param t_bgCol Background color of the tab window.
    void beginTab(const char* t_name, ImVec2 t_padding = ImVec2(0.0, 0.0),
                  ImU32 t_bgCol = IM_COL32(30, 30, 30, 255)) const;

    /// @brief End a panel tab.
    void endTab() const;

  private:
    ImVec2 m_padding = {10.f, 10.f}; ///< Default padding for the panel content.

    static constexpr float TAB_TITLE_HEIGHT = 25.0f;                            ///< Height of the tab title bar.
    static constexpr ImU32 TAB_TITLE_BG_COLOR = IM_COL32(50, 50, 50, 255);      ///< Background color of the tab title.
    static constexpr ImU32 TAB_TITLE_TEXT_COLOR = IM_COL32(230, 230, 230, 255); ///< Text color for the tab title.
    static constexpr ImVec2 TAB_TITLE_TEXT_PADDING = {10.0f, 4.5f};             ///< Padding for the tab title text.
};

} // namespace butter