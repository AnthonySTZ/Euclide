#pragma once

#include "nodes/node.h"
#include "fields/float3field.h"

namespace butter {

/// @brief Transformation parameters for a mesh node.
struct TransformSettings {
    /// @brief Translation vector (x, y, z)
    float3 translation{0.0, 0.0, 0.0};

    /// @brief Scale factors for each axis (x, y, z)
    float3 scale{1.0, 1.0, 1.0};

    /// @brief Rotation angles (in degrees) around each axis (x, y, z)
    float3 rotation{0.0, 0.0, 0.0};
};

/// @brief Node that applies translation, rotation, and scaling to a mesh.
class Transform : public Node {
  public:
    /// @brief Constructs a Transform node with fields for translation, rotation, and scale.
    Transform();

    /// @brief Applies transformations to the given mesh.
    /// @param t_mesh The mesh to transform.
    /// @param t_settings Transformation parameters (translation, rotation, scale).
    static void transform(Mesh& t_mesh, const TransformSettings& t_settings);

    /// @brief Applies scaling to the mesh.
    /// @param t_mesh The mesh to scale.
    /// @param t_scale Scale vector for each axis.
    static void scaleMesh(Mesh& t_mesh, const float3& t_scale);

    /// @brief Applies rotation to the mesh using Euler angles (degrees).
    /// @param t_mesh The mesh to rotate.
    /// @param t_rotation Rotation angles for each axis.
    static void rotateMesh(Mesh& t_mesh, const float3& t_rotation);

    /// @brief Applies translation to the mesh.
    /// @param t_mesh The mesh to translate.
    /// @param t_translation Translation vector.
    static void translateMesh(Mesh& t_mesh, const float3& t_translation);

  private:
    /// @brief Computes the output mesh by applying the node's transformation fields.
    /// @param t_index Output index (only one output in this node).
    /// @param t_inputs Vector of input meshes.
    /// @return Shared pointer to the transformed mesh.
    std::shared_ptr<Mesh> compute(const size_t t_index,
                                  const std::vector<std::shared_ptr<Mesh>>& t_inputs) const override;
};

} // namespace butter
