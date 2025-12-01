#pragma once

#include "nodes/node.h"

namespace euclide {

/// @brief Settings for the Subdivide node.
struct SubdivideSettings {
    /// @brief Number of subdivision iterations to perform.
    int divisions;
};

/// @brief Node that performs Catmull-Clark-style subdivision on a mesh.
class Subdivide : public Node {
  public:
    /// @brief Constructs a Subdivide node with 1 input, 1 output, and a "Divisions" field.
    Subdivide();

    /// @brief Subdivides a mesh using the given settings.
    /// @param t_mesh The mesh to be subdivided.
    /// @param t_settings Settings for the subdivision (number of iterations).
    static void subdivide(Mesh& t_mesh, const SubdivideSettings& t_settings);

  private:
    /// @brief Computes the output mesh of this node by subdividing the input mesh.
    /// @param t_index Output index (only one output in this node).
    /// @param t_inputs Vector of input meshes.
    /// @return Shared pointer to the subdivided output mesh.
    std::shared_ptr<Mesh> compute(const size_t t_index,
                                  const std::vector<std::shared_ptr<Mesh>>& t_inputs) const override;
};

/// @brief Returns the valence (number of connected edges) of a point in a halfedge mesh.
/// @param t_idx Index of the halfedge to inspect.
/// @param t_halfedges Vector of halfedges representing the mesh.
/// @return Number of edges connected to the given point.
[[nodiscard]] size_t valence(const size_t t_idx, const std::vector<HalfEdge>& t_halfedges);

/// @brief Determines whether a point in a halfedge mesh lies on a boundary.
/// @param t_idx Index of the halfedge to inspect.
/// @param t_halfedges Vector of halfedges representing the mesh.
/// @return True if the point is on a border; false otherwise.
[[nodiscard]] size_t isOnBorder(const size_t t_idx, const std::vector<HalfEdge>& t_halfedges);

} // namespace euclide
