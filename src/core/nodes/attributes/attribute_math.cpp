#include "attribute_math.h"

#include "fields/float4field.h"

#include <limits>

namespace euclide {

AttributeMath::AttributeMath() : Node(1, 1, "AttrMath") {
    auto kindField = std::make_shared<NodeField<int>>(0);
    kindField->setMetadata(NodeFieldMetadata{
        displayName : "Kind",
        is_combo : true,
        choices : std::move(std::vector<std::string>{"points", "primitives"})
    });
    addField("kind", kindField);

    auto firstAttribField = std::make_shared<NodeField<std::string>>("");
    firstAttribField->setMetadata(NodeFieldMetadata{
        .displayName = "Attribute A",
    });
    addField("attrA", firstAttribField);

    auto operationField = std::make_shared<NodeField<int>>(0);
    operationField->setMetadata(NodeFieldMetadata{
        displayName : "Operation",
        is_combo : true,
        choices : std::move(std::vector<std::string>{"Add", "Subtract", "Multiply", "Divide"})
    });
    addField("operation", operationField);

    auto secondAttribField = std::make_shared<NodeField<std::string>>("");
    secondAttribField->setMetadata(NodeFieldMetadata{
        .displayName = "Attribute B",
    });
    addField("attrB", secondAttribField);

    auto outputAttribField = std::make_shared<NodeField<std::string>>("");
    outputAttribField->setMetadata(NodeFieldMetadata{
        .displayName = "Output",
    });
    addField("output", outputAttribField);
}

// TODO: clean
std::shared_ptr<Mesh> AttributeMath::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) {
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>();

    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    const std::string attrAName = getField<NodeField<std::string>>("attrA")->getValue();
    const std::string attrBName = getField<NodeField<std::string>>("attrB")->getValue();
    const std::string outputAttr = getField<NodeField<std::string>>("output")->getValue();

    const Operator operation = static_cast<Operator>(getField<NodeField<int>>("operation")->getValue());

    auto attrAInfos = getAttrInfoByName(attrAName);
    auto attrBInfos = getAttrInfoByName(attrBName);
    auto outputInfos = getAttrInfoByName(outputAttr);

    if (!attrAInfos.has_value() || !attrBInfos.has_value() || !outputInfos.has_value())
        return output;

    auto attrAInfosValue = attrAInfos.value();
    auto attrBInfosValue = attrBInfos.value();
    auto attrOutInfosValue = outputInfos.value();
    bool isSingleComponent = attrAInfosValue.singleComponent;
    if (isSingleComponent != attrBInfosValue.singleComponent || isSingleComponent != attrOutInfosValue.singleComponent)
        return output;

    auto attrA = output->pointAttribs.find(attrAInfosValue.name);
    auto attrB = output->pointAttribs.find(attrBInfosValue.name);
    if (!attrA || !attrB)
        return output;

    if (isSingleComponent) {
        if (attrA->attrSize() <= attrAInfosValue.component || attrB->attrSize() <= attrBInfosValue.component)
            return output;
    }

    auto attrOut = output->pointAttribs.findOrCreate<float>(attrOutInfosValue.name, attrA->attrSize());
    if (isSingleComponent && attrOut->attrSize() <= attrOutInfosValue.component)
        return output;

    if (!isSingleComponent) {
        if (attrA->attrSize() != attrB->attrSize() || attrB->attrSize() != attrOut->attrSize())
            return output;
    }

    if (isSingleComponent) {
        auto attrAPtr = attrA->component<float>(attrAInfosValue.component);
        auto attrBPtr = attrB->component<float>(attrBInfosValue.component);
        auto attrOutPtr = attrOut->component<float>(attrOutInfosValue.component);
        switch (operation) {
        case Operator::Addition:
            addAttributes(attrAPtr, attrBPtr, attrOutPtr, output->pointAttribs.size());
            break;
        case Operator::Subtraction:
            subtractAttributes(attrAPtr, attrBPtr, attrOutPtr, output->pointAttribs.size());
            break;
        case Operator::Multiplication:
            multiplyAttributes(attrAPtr, attrBPtr, attrOutPtr, output->pointAttribs.size());
            break;
        case Operator::Division:
            divideAttributes(attrAPtr, attrBPtr, attrOutPtr, output->pointAttribs.size());
            break;
        }

        return output;
    }

    for (size_t c = 0; c < attrA->attrSize(); ++c) {
        auto attrAPtr = attrA->component<float>(c);
        auto attrBPtr = attrB->component<float>(c);
        auto attrOutPtr = attrOut->component<float>(c);
        switch (operation) {
        case Operator::Addition:
            addAttributes(attrAPtr, attrBPtr, attrOutPtr, output->pointAttribs.size());
            break;
        case Operator::Subtraction:
            subtractAttributes(attrAPtr, attrBPtr, attrOutPtr, output->pointAttribs.size());
            break;
        case Operator::Multiplication:
            multiplyAttributes(attrAPtr, attrBPtr, attrOutPtr, output->pointAttribs.size());
            break;
        case Operator::Division:
            divideAttributes(attrAPtr, attrBPtr, attrOutPtr, output->pointAttribs.size());
            break;
        }
    }

    return output;
}

void AttributeMath::addAttributes(const float* t_attrAPtr, const float* t_attrBPtr, float* t_attrOutPtr,
                                  const size_t t_count) {
    for (size_t i = 0; i < t_count; ++i) {
        t_attrOutPtr[i] = t_attrAPtr[i] + t_attrBPtr[i];
    }
}

void AttributeMath::subtractAttributes(const float* t_attrAPtr, const float* t_attrBPtr, float* t_attrOutPtr,
                                       const size_t t_count) {
    for (size_t i = 0; i < t_count; ++i) {
        t_attrOutPtr[i] = t_attrAPtr[i] - t_attrBPtr[i];
    }
}

void AttributeMath::multiplyAttributes(const float* t_attrAPtr, const float* t_attrBPtr, float* t_attrOutPtr,
                                       const size_t t_count) {
    for (size_t i = 0; i < t_count; ++i) {
        t_attrOutPtr[i] = t_attrAPtr[i] * t_attrBPtr[i];
    }
}

void AttributeMath::divideAttributes(const float* t_attrAPtr, const float* t_attrBPtr, float* t_attrOutPtr,
                                     const size_t t_count) {
    for (size_t i = 0; i < t_count; ++i) {
        if (t_attrBPtr[i] == 0) {
            t_attrOutPtr[i] =
                t_attrOutPtr[i] > 0 ? std::numeric_limits<float>::infinity() : -std::numeric_limits<float>::infinity();
        }
        t_attrOutPtr[i] = t_attrAPtr[i] / t_attrBPtr[i];
    }
}

std::optional<AttributeMath::AttributeInfo> AttributeMath::getAttrInfoByName(const std::string& t_name) {
    if (t_name.empty())
        return std::nullopt;

    AttributeInfo infos{};

    size_t nameLength = t_name.length();
    for (size_t i = 0; i < nameLength; ++i) {
        if (t_name[i] == '.') {
            if (i != nameLength - 2)
                return std::nullopt;

            const char component = t_name[i + 1];
            if (component == 'x')
                infos.component = 0;
            else if (component == 'y')
                infos.component = 1;
            else if (component == 'z')
                infos.component = 2;
            else if (component == 'w')
                infos.component = 3;
            else
                return std::nullopt;

            infos.name = t_name.substr(0, i);
            infos.singleComponent = true;
            return infos;
        }
    }

    infos.name = t_name;
    return infos;
}

} // namespace euclide