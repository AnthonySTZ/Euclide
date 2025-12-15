#pragma once

#include "geometry/mesh.h"
#include "nodes/scene.h"

namespace euclide {

class Spreadsheet {
  public:
    enum class AttributeKind { POINTS, PRIMITIVES };

  public:
    Spreadsheet(std::weak_ptr<Scene> t_scene);

    [[nodiscard]] inline const std::shared_ptr<Mesh> mesh() noexcept { return m_mesh.lock(); }
    [[nodiscard]] inline AttributeKind kind() noexcept { return m_kind; }

  private:
    std::weak_ptr<Mesh> m_mesh;

    AttributeKind m_kind = AttributeKind::POINTS;
};

} // namespace euclide