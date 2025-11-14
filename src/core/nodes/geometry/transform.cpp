#include "transform.h"

namespace butter {
    
std::shared_ptr<Mesh> Transform::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>> &t_inputs)
{
    //TODO:
    if (t_inputs[0] == nullptr) return std::make_shared<Mesh>();
    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    float3 position = getField<Float3Field>("translate")->getValue();
    float3 size = getField<Float3Field>("size")->getValue();
    float3 rotate = getField<Float3Field>("rotate")->getValue();

    return output;
}

}