#pragma once

#include "nodes/scene.h"
#include "panel.h"
#include "rendering/renderer.h"

#include <memory>
#include <chrono>
#include <string>

namespace butter {

/// @brief Panel displaying the 3D viewport of the scene.
///
/// Handles rendering of the scene and a grid, camera controls, and keyboard/mouse
/// interaction for toggling rendering modes (primitives, wireframe, points, grid).
class Viewport : public Panel {
  public:
    /// @brief Construct a Viewport panel attached to a Scene.
    /// @param t_scene Weak pointer to the Scene to visualize.
    Viewport(const std::shared_ptr<Scene>& t_scene);

    ~Viewport() = default;

    /// @brief Draw the viewport panel.
    void draw() override;

  private:
    /// @brief Handle mouse input for camera interaction and viewport clicks.
    void handleMouse();

    /// @brief Handle keyboard input for toggling rendering modes and camera retargeting.
    void handleKeys();

    /// @brief Re-target the camera to the currently rendered node, if available.
    void retargetCamera();

    /// @brief Move the camera based on mouse drag input.
    void moveCamera();

    /// @brief Render the scene and grid to the framebuffer and display it in ImGui.
    void drawRender();

    /// @brief Check if the viewport has been resized and update framebuffer accordingly.
    void checkForResize();

    /// @brief Draw overlay information such as FPS and keyboard shortcuts.
    void drawInfos() const;

  private:
    std::weak_ptr<Scene> m_scene; ///< Scene associated with this viewport.

    ImVec2 m_windowPosition; ///< Position of the viewport window in ImGui.

    // Rendering resources
    std::unique_ptr<Renderer> m_renderer;       ///< Renderer instance.
    RenderModel m_renderModel;                  ///< Main scene render model.
    RenderModel m_gridModel;                    ///< Grid render model.
    uint32_t m_viewportWidth, m_viewportHeight; ///< Current viewport size.

    ImVec2 m_padding{0, 0};           ///< Internal padding for the panel.
    std::shared_ptr<Camera> m_camera; ///< Camera used for the viewport.

    // Mouse interaction state
    bool m_isItemHovered = false;
    bool m_isLeftClicked = false;
    bool m_isMiddleClicked = false;
    bool m_isRightClicked = false;

    bool m_showGrid = true; ///< Whether to display the grid.
};

} // namespace butter