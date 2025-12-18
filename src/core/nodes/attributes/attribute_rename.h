#pragma once

#include "nodes/node.h"

namespace euclide {

class AttributeRename : public Node {
  public:
    enum Kind { POINTS = 0, PRIMITIVES = 1 };

    AttributeRename();

    static void renameAttribute(AttributeSet& t_attribs, const std::string& t_name, const std::string& t_newName);

  private:
    std::shared_ptr<Mesh> compute(const size_t t_index,
                                  const std::vector<std::shared_ptr<Mesh>>& t_inputs) const override;
};

} // namespace euclide
