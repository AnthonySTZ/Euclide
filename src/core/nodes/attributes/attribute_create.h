#pragma once

#include "nodes/node.h"

namespace euclide {

class AttributeCreate : public Node {
  public:
    AttributeCreate();

  private:
    std::shared_ptr<Mesh> compute(const size_t t_index,
                                  const std::vector<std::shared_ptr<Mesh>>& t_inputs) const override;
};

} // namespace euclide
