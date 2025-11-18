#include "transform.h"

#include "utils/math.h"

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

    //Scale
    for (size_t i = 0; i < points.size(); ++i) {
        points.posX[i] *= size[0];
        points.posY[i] *= size[1];
        points.posZ[i] *= size[2];
    }

    //Rotate
    const double rotX = radians(rotate[0]);
    const double rotY = radians(rotate[1]);
    const double rotZ = radians(rotate[2]);

    const double cosX = std::cos(rotX);
    const double cosY = std::cos(rotY);
    const double cosZ = std::cos(rotZ);

    const double sinX = std::sin(rotX);
    const double sinY = std::sin(rotY);
    const double sinZ = std::sin(rotZ);

    const double row_1[3] = {
        cosY * cosX, (sinZ * sinY * cosX) - (cosZ * sinX), (cosZ * sinY * cosX) + (sinZ * sinX)
    };
    const double row_2[3] = {
        cosY * sinX, (sinZ * sinY * sinX) + (cosZ * cosX), (cosZ * sinY * sinX) - (sinZ * cosX)
    };
    const double row_3[3] = {
        -sinY, sinZ * cosY, cosZ * cosY
    };

    for (size_t i = 0; i < points.size(); ++i) {
        const float posX = points.posX[i];
        const float posY = points.posY[i];
        const float posZ = points.posZ[i];
        points.posX[i] = posX * row_1[0] + posY * row_1[1] + posZ * row_1[2];
        points.posY[i] = posX * row_2[0] + posY * row_2[1] + posZ * row_2[2];
        points.posZ[i] = posX * row_3[0] + posY * row_3[1] + posZ * row_3[2];
    }


    //Translate
    for (size_t i = 0; i < points.size(); ++i) {
        points.posX[i] += translate[0];
        points.posY[i] += translate[1];
        points.posZ[i] += translate[2];
    }

    return output;
}

}