#pragma once

#include "nodes/node.h"
#include "fields/float3field.h"

namespace butter {

/// @brief Parameters to define a cube mesh.
struct CubeSettings {
    /// @brief Center position of the cube.
    float3 position;

    /// @brief Size along each axis (width, height, depth).
    float3 size;
};

/// @brief Node that generates a cube mesh.
class Cube : public Node {
  public:
    /// @brief Constructs a Cube node with fields for position and size.
    Cube();

    /// @brief Creates a cube mesh according to the given settings.
    /// @param t_mesh The mesh object to populate.
    /// @param t_settings The position and size of the cube.
    static void createCube(Mesh& t_mesh, const CubeSettings& t_settings);

  private:
    /// @brief Computes the output cube mesh.
    /// @param t_index Output index (only one output in this node).
    /// @param t_inputs Input meshes (not used for Cube node).
    /// @return Shared pointer to the generated cube mesh.
    std::shared_ptr<Mesh> compute(const size_t t_index,
                                  const std::vector<std::shared_ptr<Mesh>>& t_inputs) const override;
};

} // namespace butter
