#pragma once

#include "nodes/node.h"
#include <optional>

namespace euclide {

class AttributeMath : public Node {
  public:
    enum Kind { POINTS = 0, PRIMITIVES = 1 };
    enum Operator {
        Addition = 0,
        Subtraction = 1,
        Multiplication = 2,
        Division = 3,
    };

    struct AttributeInfo {
        std::string name = "";
        bool singleComponent = false;
        int component = 0;
    };

    AttributeMath();

    static void addAttributes(const float* t_attrAPtr, const float* t_attrBPtr, float* t_attrOutPtr,
                              const size_t t_count);

    void subtractAttributes(const float* t_attrAPtr, const float* t_attrBPtr, float* t_attrOutPtr,
                            const size_t t_count);

    void multiplyAttributes(const float* t_attrAPtr, const float* t_attrBPtr, float* t_attrOutPtr,
                            const size_t t_count);

    void divideAttributes(const float* t_attrAPtr, const float* t_attrBPtr, float* t_attrOutPtr, const size_t t_count);

  private:
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) override;

    static std::optional<AttributeMath::AttributeInfo> getAttrInfoByName(const std::string& t_name);
};

} // namespace euclide
