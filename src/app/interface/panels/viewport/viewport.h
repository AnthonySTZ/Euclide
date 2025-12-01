#pragma once

#include "nodes/scene.h"

#include "rendering/camera.h"
#include "rendering/render_model.h"

namespace butter {

class Viewport {
  public:
    Viewport(const std::weak_ptr<Scene> t_scene);
    ~Viewport() = default;

  private:
    std::weak_ptr<Scene> m_scene;

    RenderModel m_renderModel; ///< Main scene render model.
    RenderModel m_gridModel;
    Camera m_camera;
};

} // namespace butter