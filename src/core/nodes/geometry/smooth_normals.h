#pragma once

#include "nodes/node.h"
#include "fields/float3field.h"

namespace euclide {

/// @brief Node that applies translation, rotation, and scaling to a mesh.
class SmoothNormals : public Node {
  public:
    SmoothNormals();

    /// @brief Applies translation to the mesh.
    /// @param t_mesh The mesh to translate.
    /// @param t_translation Translation vector.
    static void smoothNormals(Mesh& t_mesh);

  private:
    std::shared_ptr<Mesh> compute(const size_t t_index,
                                  const std::vector<std::shared_ptr<Mesh>>& t_inputs) const override;
};

} // namespace euclide
