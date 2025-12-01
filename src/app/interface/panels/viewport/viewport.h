#pragma once

#include "nodes/scene.h"

#include "rendering/camera.h"
#include "rendering/render_model.h"

namespace butter {

class Viewport {
  public:
    Viewport(const std::weak_ptr<Scene> t_scene);
    ~Viewport() = default;

    [[nodiscard]] inline std::shared_ptr<Camera> camera() const noexcept { return m_camera; }
    [[nodiscard]] inline const RenderModel& renderModel() const noexcept { return m_renderModel; }
    [[nodiscard]] inline const RenderModel& gridModel() const noexcept { return m_gridModel; }

  private:
    std::weak_ptr<Scene> m_scene = std::weak_ptr<Scene>();

    RenderModel m_renderModel{}; ///< Main scene render model.
    RenderModel m_gridModel{};
    std::shared_ptr<Camera> m_camera;
};

} // namespace butter