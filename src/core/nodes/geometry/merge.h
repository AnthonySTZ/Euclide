#pragma once

#include "nodes/node.h"

namespace butter {

/// @brief Node that merges two input meshes into a single output mesh.
class Merge : public Node {
  public:
    /// @brief Constructs a Merge node with 2 inputs and 1 output.
    Merge();

    /// @brief Merges the contents of two meshes into the first mesh.
    /// @param t_mesh Mesh that will receive the merged data.
    /// @param t_mesh_2 Mesh to merge into t_mesh.
    static void merge(Mesh& t_mesh, const Mesh& t_mesh_2);

  private:
    /// @brief Computes the output mesh of this node by merging its two input meshes.
    /// @param t_index Output index (only one output in this node).
    /// @param t_inputs Vector of input meshes.
    /// @return Shared pointer to the merged output mesh.
    std::shared_ptr<Mesh> compute(const size_t t_index,
                                  const std::vector<std::shared_ptr<Mesh>>& t_inputs) const override;
};

} // namespace butter
