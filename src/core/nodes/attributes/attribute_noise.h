#pragma once

#include "nodes/node.h"

namespace euclide {

class AttributeNoise : public Node {
  public:
    enum Kind { POINTS = 0, PRIMITIVES = 1 };

    AttributeNoise();

    static void perlinNoise(AttributeSet& t_attribs, const std::string& t_name, const int t_attrSize,
                            const int t_octaves, const float persistence);

  private:
    std::shared_ptr<Mesh> compute(const size_t t_index,
                                  const std::vector<std::shared_ptr<Mesh>>& t_inputs) const override;
};

} // namespace euclide
