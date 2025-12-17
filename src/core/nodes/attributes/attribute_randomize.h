#pragma once

#include "nodes/node.h"

namespace euclide {

class AttributeRandomize : public Node {
  public:
    enum Kind { POINTS = 0, PRIMITIVES = 1 };

    AttributeRandomize();

    static void randomizeAttribute(AttributeSet& t_attribs, const std::string& t_name, const int t_attrSize,
                                   const float4 t_minValues, const float4 t_maxValues, const float t_seed);

  private:
    std::shared_ptr<Mesh> compute(const size_t t_index,
                                  const std::vector<std::shared_ptr<Mesh>>& t_inputs) const override;
};

} // namespace euclide
