#include "attribute_math.h"

#include "fields/float4field.h"

namespace euclide {

AttributeMath::AttributeMath() : Node(1, 1, "AttrMath") {
    auto kindField = std::make_shared<NodeField<int>>(0);
    kindField->setMetadata(NodeFieldMetadata{
        displayName : "Kind",
        is_combo : true,
        choices : std::move(std::vector<std::string>{"points", "primitives"})
    });
    addField("kind", kindField);

    auto attributeField = std::make_shared<NodeField<std::string>>("Cd");
    attributeField->setMetadata(NodeFieldMetadata{displayName : "Attribute Name"});
    addField("attributeName", attributeField);

    auto sizeField = std::make_shared<NodeField<int>>(0);
    sizeField->setMetadata(NodeFieldMetadata{
        displayName : "Attribute Size",
        min : 0,
        max : 4,
        step : 1,
    });
    addField("size", sizeField);

    auto valueField = std::make_shared<Float4Field>(0.0f, 0.0f, 0.0f, 0.0f);
    valueField->setMetadata(NodeFieldMetadata{displayName : "Attribute Value", step : 0.01f});
    addField("value", valueField);
}

std::shared_ptr<Mesh> AttributeMath::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) {
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>();

    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    return output;
}

} // namespace euclide