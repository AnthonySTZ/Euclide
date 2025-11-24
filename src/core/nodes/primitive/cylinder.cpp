#include "cylinder.h"

namespace butter {

 float3 position = {0.0, 0.0, 0.0};
    float2 radius = {1.0, 1.0};
    float height = 1.0;
    int divisions = 8;
    bool capped = true; 

Cylinder::Cylinder()
    : Node(0, 1, "Cylinder") 
{
    auto positionField = std::make_shared<Float3Field>(0.0, 0.0, 0.0);
    positionField->setMetadata(NodeFieldMetadata{
        displayName: "Position",
        step: 0.02f
    });
    addField("position", positionField);
    
    auto radiusField = std::make_shared<Float2Field>(1.0, 1.0);
    radiusField->setMetadata(NodeFieldMetadata{
        displayName: "Radius",
        min: 0.0f,
        step: 0.02f
    });
    addField("radius", radiusField);
    
    auto heightField = std::make_shared<NodeField<float>>(1.0);
    heightField->setMetadata(NodeFieldMetadata{
        displayName: "Height",
        min: 0.0f,
        step: 0.02f
    });
    addField("height", heightField);
    
    auto divisionsField = std::make_shared<NodeField<int>>(8);
    divisionsField->setMetadata(NodeFieldMetadata{
        displayName: "Divisions",
        min: 3,
        step: 1
    });
    addField("divisions", divisionsField);
    
    auto cappedField = std::make_shared<NodeField<bool>>(true);
    cappedField->setMetadata(NodeFieldMetadata{
        displayName: "Capped"
    });
    addField("capped", cappedField);
}

std::shared_ptr<Mesh> Cylinder::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>> &t_inputs)
{
    auto output = std::make_shared<Mesh>();

    float3 position = getField<Float3Field>("position")->getValue();
    float2 radius = getField<Float2Field>("radius")->getValue();
    
    float height = getField<NodeField<float>>("height")->getValue();
    int divisions = getField<NodeField<int>>("divisions")->getValue();
    bool capped = getField<NodeField<bool>>("capped")->getValue();

    const CylinderSettings settings{
        .position = position,
        .radius = radius,
        .height = height,
        .divisions = divisions,
        .capped = capped
    };

    createCylinder(*output, settings);

    return output;
}

void Cylinder::createCylinder(Mesh &t_mesh, const CylinderSettings &t_settings)
{
    //TODO:
}

}