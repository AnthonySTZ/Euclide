#pragma once

#include "nodes/node.h"
#include "fields/float3field.h"

namespace euclide {

/// @brief Parameters to define a quadsphere mesh.
struct QuadSphereSettings {
    /// @brief Center position of the quadsphere.
    float3 position;

    /// @brief Size along each axis (width, height, depth).
    float3 size;

    /// @brief Number of subdivision iterations to perform.
    int divisions;
};

/// @brief Node that generates a quadsphere mesh.
class QuadSphere : public Node {
  public:
    /// @brief Constructs a QuadSphere node with fields for position and size.
    QuadSphere();

    /// @brief Creates a quadsphere mesh according to the given settings.
    /// @param t_mesh The mesh object to populate.
    /// @param t_settings The position and size of the quadsphere.
    static void createQuadSphere(Mesh& t_mesh, const QuadSphereSettings& t_settings);

  private:
    /// @brief Computes the output quadsphere mesh.
    /// @param t_index Output index (only one output in this node).
    /// @param t_inputs Input meshes (not used for QuadSphere node).
    /// @return Shared pointer to the generated quadsphere mesh.
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) override;
};

} // namespace euclide
