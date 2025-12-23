#pragma once

#include "nodes/node.h"

namespace euclide {

class Null : public Node {
  public:
    Null();

  private:
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) override;
};

} // namespace euclide
