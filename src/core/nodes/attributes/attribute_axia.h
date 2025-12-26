#pragma once

#include "nodes/node.h"

namespace euclide {

class AttributeAXIA : public Node {
  public:
    enum Kind { POINTS = 0, PRIMITIVES = 1 };

    AttributeAXIA();

    static void computeAXIA(AttributeSet& t_attribs, const std::string& t_script);

  private:
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) override;
};

} // namespace euclide
