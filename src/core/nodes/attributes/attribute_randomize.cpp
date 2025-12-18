#include "attribute_randomize.h"

#include "fields/float4field.h"

#include "utils/random.h"

namespace euclide {

AttributeRandomize::AttributeRandomize() : Node(1, 1, "AttrRandomize") {
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

    auto minValueField = std::make_shared<Float4Field>(0.0f, 0.0f, 0.0f, 0.0f);
    minValueField->setMetadata(NodeFieldMetadata{displayName : "Min Value", step : 0.01f});
    addField("minValue", minValueField);

    auto maxValueField = std::make_shared<Float4Field>(1.0f, 1.0f, 1.0f, 1.0f);
    maxValueField->setMetadata(NodeFieldMetadata{displayName : "Max Value", step : 0.01f});
    addField("maxValue", maxValueField);

    auto seedField = std::make_shared<NodeField<int>>(0.0f);
    seedField->setMetadata(NodeFieldMetadata{displayName : "Seed", min : 0.0f, step : 1.0f});
    addField("seed", seedField);
}

std::shared_ptr<Mesh> AttributeRandomize::compute(const size_t t_index,
                                                  const std::vector<std::shared_ptr<Mesh>>& t_inputs) const {
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>();

    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    const int attrSize = getField<NodeField<int>>("size")->getValue();
    if (attrSize < 1)
        return output;

    const Kind kind = static_cast<Kind>(getField<NodeField<int>>("kind")->getValue());
    const std::string attrName = getField<NodeField<std::string>>("attributeName")->getValue();
    const float4 minValue = getField<Float4Field>("minValue")->getValue();
    const float4 maxValue = getField<Float4Field>("maxValue")->getValue();
    const int seed = getField<NodeField<int>>("seed")->getValue();

    if (attrName.empty())
        return output;

    if (kind == Kind::POINTS) {
        randomizeAttribute(output->pointAttribs, attrName, attrSize, minValue, maxValue, seed);
    } else if (kind == Kind::PRIMITIVES) {
        randomizeAttribute(output->primAttribs, attrName, attrSize, minValue, maxValue, seed);
    }

    return output;
}

void AttributeRandomize::randomizeAttribute(AttributeSet& t_attribs, const std::string& t_name, const int t_attrSize,
                                            const float4 t_minValues, const float4 t_maxValues, const int t_seed) {
    auto attr = t_attribs.findOrCreate<float>(t_name, t_attrSize);
    const size_t attrSize = std::min(std::min(attr->attrSize(), t_attrSize), 4);
    const size_t numElements = attr->size();

    auto randomGen = RandomGenerator(t_seed);
    for (size_t c = 0; c < attrSize; ++c) {
        const float min = t_minValues[c];
        const float max = t_maxValues[c];
        float* __restrict ptr = attr->component<float>(c);
        for (size_t i = 0; i < numElements; ++i) {
            ptr[i] = randomGen.random(min, max);
        }
    }
}

} // namespace euclide