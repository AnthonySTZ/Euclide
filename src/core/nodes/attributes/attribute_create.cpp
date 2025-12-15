#include "attribute_create.h"

#include "fields/float4field.h"

namespace euclide {

AttributeCreate::AttributeCreate() : Node(1, 1, "AttrCreate") {
    auto kindField = std::make_shared<NodeField<int>>(0);
    kindField->setMetadata(NodeFieldMetadata{
        displayName : "Kind",
        is_combo : true,
        choices : std::move(std::vector<std::string>{"points", "primitives"})
    });
    addField("kind", kindField);

    auto attributeField = std::make_shared<NodeField<std::string>>("");
    attributeField->setMetadata(NodeFieldMetadata{displayName : "Attribute Name"});
    addField("attributeName", attributeField);

    auto sizeField = std::make_shared<NodeField<int>>(1);
    sizeField->setMetadata(NodeFieldMetadata{
        displayName : "Attribute Size",
        min : 1,
        max : 4,
        step : 1,
    });
    addField("size", sizeField);

    auto valueField = std::make_shared<Float4Field>(0.0f, 0.0f, 0.0f, 0.0f);
    valueField->setMetadata(NodeFieldMetadata{displayName : "Attribute Value", step : 0.01f});
    addField("value", valueField);
}

std::shared_ptr<Mesh> AttributeCreate::compute(const size_t t_index,
                                               const std::vector<std::shared_ptr<Mesh>>& t_inputs) const {
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>();

    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    const Kind kind = static_cast<Kind>(getField<NodeField<int>>("kind")->getValue());
    const std::string attrName = getField<NodeField<std::string>>("attributeName")->getValue();
    const int attrSize = getField<NodeField<int>>("size")->getValue();
    const float4 attrValue = getField<Float4Field>("value")->getValue();

    if (attrName.empty())
        return output;

    if (kind == Kind::POINTS) {
        createAttribute(output->pointAttribs, attrName, attrSize, attrValue);
    } else if (kind == Kind::PRIMITIVES) {
        createAttribute(output->primAttribs, attrName, attrSize, attrValue);
    }

    return output;
}

void AttributeCreate::createAttribute(AttributeSet& t_attribs, const std::string& t_name, const int t_attrSize,
                                      const float4 t_defaultValue) {
    auto attr = t_attribs.findOrCreate<float>(t_name, t_attrSize);
    const size_t attrSize =
        std::min(std::min(attr->attrSize(), t_attrSize), 4); // check if already exists with a smaller size
    const size_t numElements = attr->size();
    for (size_t c = 0; c < attrSize; ++c) {
        float* ptr = attr->component<float>(c);
        float value = t_defaultValue[c];
        for (size_t i = 0; i < numElements; ++i) {
            ptr[i] = value;
        }
    }
}

} // namespace euclide