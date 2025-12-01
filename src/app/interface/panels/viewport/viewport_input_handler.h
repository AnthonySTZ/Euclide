#pragma once

#include "viewport.h"
#include "interface/utils/imgui_utils.h"

namespace euclide {

/// @brief Handles all user inputs inside the Viewport panel.
///
/// This class processes mouse and keyboard events specifically
/// when the viewport is hovered. It translates user interactions
/// into camera movement (orbit, pan, dolly) and render mode toggles.
/// Primary responsibilities:
/// - Mouse input for camera control
/// - Keyboard input for render settings & camera reset
/// - Ensures inputs only apply when viewport is focused/hovered
class ViewportInputHandler {
  public:
    /// @brief Construct a Viewport input handler.
    /// @param t_viewport Weak pointer to the viewport controlled by user input.
    ViewportInputHandler(const std::weak_ptr<Viewport> t_viewport) : m_viewport(t_viewport) {}
    ~ViewportInputHandler() = default;

    /// @brief Polls and dispatches input events every frame.
    /// Should be called inside the viewport panel draw loop.
    void pollEvents();

  private:
    /// @brief Process mouse interactions (orbit, pan, dolly).
    /// - LMB drag -> orbit camera
    /// - MMB drag -> pan camera
    /// - RMB drag -> dolly / zoom camera
    /// Only active if viewport is hovered.
    void handleMouseInputs();

    /// @brief Process keyboard commands for scene visualization.
    /// Available shortcuts (while hovered):
    /// - **P** -> Toggle primitive rendering
    /// - **W** -> Toggle wireframe
    /// - **V** -> Toggle point mode
    /// - **G** -> Toggle grid visibility
    /// - **F** -> Reset/retarget camera
    void handleKeyInputs();

  private:
    std::weak_ptr<Viewport> m_viewport; ///< Viewport controlled by input

    bool m_isWindowHovered = false;            ///< True when mouse is over viewport region
    bool m_isLeftMouseButtonClicked = false;   ///< True while LMB held
    bool m_isMiddleMouseButtonClicked = false; ///< True while MMB held
    bool m_isRightMouseButtonClicked = false;  ///< True while RMB held
};

} // namespace euclide
