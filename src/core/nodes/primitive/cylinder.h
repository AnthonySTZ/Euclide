#pragma once

#include "nodes/node.h"
#include "fields/float3field.h"
#include "fields/float2field.h"

namespace euclide {

/// @brief Parameters defining a cylinder mesh.
struct CylinderSettings {
    /// @brief Center position of the cylinder.
    float3 position{0.0, 0.0, 0.0};

    /// @brief Top and bottom radius.
    float2 radius{1.0, 1.0};

    /// @brief Height along the Y axis.
    float height{1.0};

    /// @brief Number of divisions around the circumference.
    int divisions{8};

    /// @brief Whether to cap the cylinder with top and bottom faces.
    bool capped{true};
};

/// @brief Node that generates a cylinder mesh.
class Cylinder : public Node {
  public:
    /// @brief Constructs a Cylinder node with fields for position, radius, height, divisions, and capped.
    Cylinder();

    /// @brief Creates a cylinder mesh according to the given settings.
    /// @param t_mesh The mesh object to populate.
    /// @param t_settings The position, radius, height, divisions, and capped flag of the cylinder.
    static void createCylinder(Mesh& t_mesh, const CylinderSettings& t_settings);

  private:
    /// @brief Computes the output cylinder mesh.
    /// @param t_index Output index (only one output in this node).
    /// @param t_inputs Input meshes (not used for Cylinder node).
    /// @return Shared pointer to the generated cylinder mesh.
    std::shared_ptr<Mesh> compute(const size_t t_index,
                                  const std::vector<std::shared_ptr<Mesh>>& t_inputs) const override;
};

} // namespace euclide
