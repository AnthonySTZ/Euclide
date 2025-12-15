#pragma once

#include "geometry/mesh.h"
#include "nodes/scene.h"

namespace euclide {

class Spreadsheet {
  public:
    Spreadsheet(std::weak_ptr<Scene> t_scene);

  private:
    std::weak_ptr<Mesh> m_mesh;
};

} // namespace euclide