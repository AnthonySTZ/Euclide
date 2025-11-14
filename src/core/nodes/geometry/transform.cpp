#include "transform.h"

namespace butter {
    
Transform::Transform()
    : Node(1, 1, "Transform")
{
    auto translateField = std::make_shared<Float3Field>(0.0, 0.0, 0.0);
    translateField->setMetadata(NodeFieldMetadata{
        displayName: "Translate",
        step: 0.02f
    });
    addField("translate", translateField);
    
    auto sizeField = std::make_shared<Float3Field>(1.0, 1.0, 1.0);
    sizeField->setMetadata(NodeFieldMetadata{
        displayName: "Size",
        step: 0.02f
    });
    addField("size", sizeField);

    auto rotateField = std::make_shared<Float3Field>(0.0, 0.0, 0.0);
    rotateField->setMetadata(NodeFieldMetadata{
        displayName: "Rotate",
        step: 0.02f
    });
    addField("rotate", rotateField);
}

std::shared_ptr<Mesh> Transform::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>> &t_inputs)
{
    if (t_inputs[0] == nullptr) return std::make_shared<Mesh>();
    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    float3 translate = getField<Float3Field>("translate")->getValue();
    float3 size = getField<Float3Field>("size")->getValue();
    float3 rotate = getField<Float3Field>("rotate")->getValue();

    auto& points = output->points;

    for (size_t i = 0; i < points.size(); ++i) {
        points.posX[i] *= size[0];
        points.posY[i] *= size[1];
        points.posZ[i] *= size[2];
    }

    return output;
}

}