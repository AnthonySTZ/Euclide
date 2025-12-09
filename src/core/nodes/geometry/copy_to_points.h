#pragma once

#include "nodes/node.h"

namespace euclide {

class CopyToPoints : public Node {
  public:
    CopyToPoints();
    static void copyToPoints(Mesh& t_mesh, const Mesh& t_points);

  private:
    std::shared_ptr<Mesh> compute(const size_t t_index,
                                  const std::vector<std::shared_ptr<Mesh>>& t_inputs) const override;
};

} // namespace euclide
