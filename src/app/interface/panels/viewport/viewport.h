#pragma once

#include "nodes/scene.h"
#include "rendering/camera.h"
#include "rendering/render_model.h"

namespace euclide {

/// @brief Represents a viewport used to visualize and interact with a scene.
///
/// The viewport maintains its own camera and rendering resources.
/// It observes the scene to update its content when meshes change.
class Viewport {
  public:
    /// @brief Constructs a viewport linked to a scene.
    /// @param scene Weak reference to the scene that will feed geometry and update notifications.
    Viewport(std::weak_ptr<Scene> scene);

    /// @brief Access the camera controlling the viewport.
    /// @return Shared pointer to the camera object.
    [[nodiscard]] std::shared_ptr<Camera> camera() { return m_camera; }

    /// @brief Returns the render model used to draw the main scene.
    /// @return Reference to the main render model.
    [[nodiscard]] RenderModel& renderModel() noexcept { return m_renderModel; }

    /// @brief Returns the render model responsible for drawing the grid overlay.
    /// @return Reference to the grid model.
    [[nodiscard]] std::vector<RenderModel>& viewportModels() noexcept { return m_viewportModels; }

    /// @brief Moves camera focus to the current render node center.
    ///        Called when geometry changes or user requests "frame all".
    void retargetCamera();

  private:
    void setViewportModels();

  private:
    /// Weak link to the scene used for mesh updates.
    std::weak_ptr<Scene> m_scene;

    /// Render model responsible for visualizing scene geometry.
    RenderModel m_renderModel;

    /// Render model for viewport display.
    std::vector<RenderModel> m_viewportModels;

    /// Camera used to view the viewport.
    std::shared_ptr<Camera> m_camera;
};

} // namespace euclide
