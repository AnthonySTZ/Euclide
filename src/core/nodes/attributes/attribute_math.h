#pragma once

#include "nodes/node.h"
#include <optional>

namespace euclide {

class AttributeMath : public Node {
  public:
    enum Kind { POINTS = 0, PRIMITIVES = 1 };
    struct AttributeInfo {
        std::string name = "";
        bool singleComponent = false;
        int component = 0;
    };

    AttributeMath();

    static void addAttributes(AttributeSet& t_attribs, const std::string& t_firstAttrib,
                              const std::string& t_secondAttrib, const std::string& t_resultAttrib);

  private:
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) override;

    static std::optional<AttributeMath::AttributeInfo> getAttrInfoByName(const std::string& t_name);
};

} // namespace euclide
