#pragma once

#include "viewport.h"
#include "rendering/renderer.h"

namespace euclide {

/// @brief Responsible for rendering a Viewport panel to an ImGui window.
///
/// The ViewportRenderer manages the Renderer instance, handles framebuffer
/// resizing when the window changes size, and draws scene content every frame.
/// Additionally, it overlays real-time statistics (FPS, render counts, modes)
/// directly on top of the rendered viewport.
///
/// Responsibilities:
/// - Create and configure a Renderer
/// - Resize framebuffer when the viewport size changes
/// - Draw grid + scene RenderModel
/// - Display performance and debug information
class ViewportRenderer {
  public:
    /// @brief Construct a renderer for a given viewport.
    /// @param t_viewport Weak ptr to a viewport panel this renderer draws.
    ViewportRenderer(const std::weak_ptr<Viewport> t_viewport);
    ~ViewportRenderer() = default;

    /// @brief Main entry point — renders viewport content + debug overlays.
    ///
    /// Call this once per frame inside the Viewport::draw() function.
    void render();

  private:
    /// @brief Detects viewport size changes and resizes framebuffer accordingly.
    void checkForResize();

    /// @brief Renders the scene (grid, model) to the framebuffer and draws it in ImGui.
    void draw();

    /// @brief Draws performance and context information in the viewport.
    void drawInfos();

    /// @brief Draws a single info line onto the viewport overlay.
    /// @param t_infos Text to draw.
    void addInfos(const std::string& t_infos);

  private:
    std::weak_ptr<Viewport> m_viewport;   ///< Viewport this renderer draws from.
    std::unique_ptr<Renderer> m_renderer; ///< Rendering backend.

    uint32_t m_viewportWidth = 0;  ///< Current viewport width.
    uint32_t m_viewportHeight = 0; ///< Current viewport height.

    ImVec2 m_windowPosition{0, 0}; ///< Start position where info text is drawn.
    ImVec2 m_infoOffset{0, 0};     ///< Vertical offset for each info line.

    static constexpr ImVec2 INFO_POS = ImVec2(10, 30);
    static constexpr ImU32 INFO_COLOR = IM_COL32(255, 255, 255, 255);
    static constexpr ImVec2 INFO_PADDING = ImVec2(0, 25);
};

} // namespace euclide
