#pragma once

#include "nodes/scene.h"

namespace butter {

class Viewport {
  public:
    Viewport(const std::weak_ptr<Scene> t_scene) : m_scene(t_scene) {}
    ~Viewport() = default;

  private:
    std::weak_ptr<Scene> m_scene;
};

} // namespace butter