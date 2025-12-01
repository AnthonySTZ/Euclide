#pragma once

#include "interface/panels/panel.h"
#include "interface/panels/viewport/viewport.h"
#include "interface/panels/viewport/viewport_renderer.h"
#include "interface/panels/viewport/viewport_input_handler.h"

namespace butter {

/// @brief UI panel responsible for displaying and interacting with the scene viewport.
///
/// This panel embeds:
/// - A Viewport (camera, render models, scene link)
/// - A ViewportRenderer (framebuffer, drawing, overlays)
/// - A ViewportInputHandler (mouse/keyboard navigation)
///
/// It acts as the ImGui wrapper for the interactive 3D window.
class ViewportPanel : public Panel {
  public:
    /// @brief Construct a viewport panel bound to a scene.
    /// @param scene Weak pointer to the scene that will be visualized.
    ViewportPanel(std::weak_ptr<Scene> scene);

    /// @brief Draw the viewport panel UI.
    ///
    /// Handles user input → then renders the scene.
    void draw() override;

  private:
    /// Padding applied inside the panel window.
    ImVec2 m_padding{0, 0};

    /// 3D viewport (camera + render models).
    std::shared_ptr<Viewport> m_viewport;

    /// Handles framebuffer drawing and info overlay.
    ViewportRenderer m_viewportRenderer;

    /// Manages orbit/pan/dolly and keyboard shortcuts.
    ViewportInputHandler m_viewportInputHandler;
};

} // namespace butter
