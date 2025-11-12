#include "transform.h"

namespace butter {
    
std::shared_ptr<Mesh> Transform::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>> &t_inputs)
{
    if (t_inputs[0] == nullptr) return std::make_shared<Mesh>();
    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    vec3 position = getField<Float3Field>("position")->getValue();
    vec3 size = getField<Float3Field>("size")->getValue();
    vec3 rotate = getField<Float3Field>("rotate")->getValue();

    return output;
}

}