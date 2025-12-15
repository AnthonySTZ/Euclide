#pragma once

#include "geometry/mesh.h"
#include "nodes/scene.h"

namespace euclide {

class Spreadsheet {
  public:
    Spreadsheet(std::weak_ptr<Scene> t_scene);

    [[nodiscard]] inline const std::shared_ptr<Mesh> mesh() noexcept { return m_mesh.lock(); }

  private:
    std::weak_ptr<Mesh> m_mesh;
};

} // namespace euclide