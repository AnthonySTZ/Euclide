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

std::shared_ptr<Mesh> AttributeMath::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) {
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>();

    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    const std::string attrA = getField<NodeField<std::string>>("attrA")->getValue();
    const std::string attrB = getField<NodeField<std::string>>("attrB")->getValue();
    const std::string outputAttr = getField<NodeField<std::string>>("output")->getValue();

    addAttributes(output->pointAttribs, attrA, attrB, outputAttr);

    return output;
}

void AttributeMath::addAttributes(AttributeSet& t_attribs, const std::string& t_firstAttrib,
                                  const std::string& t_secondAttrib, const std::string& t_resultAttrib) {
    auto attrAInfos = getAttrInfoByName(t_firstAttrib);
    auto attrBInfos = getAttrInfoByName(t_secondAttrib);
    auto outputInfos = getAttrInfoByName(t_resultAttrib);

    if (!attrAInfos.has_value() || !attrBInfos.has_value() || !outputInfos.has_value())
        return;

    auto attrAInfosValue = attrAInfos.value();
    auto attrBInfosValue = attrBInfos.value();
    auto outputInfosValue = outputInfos.value();
    bool isSingleComponent = attrAInfosValue.singleComponent;
    if (isSingleComponent != attrBInfosValue.singleComponent || isSingleComponent != outputInfosValue.singleComponent)
        return;

    auto attrA = t_attribs.find(attrAInfosValue.name);
    auto attrB = t_attribs.find(attrBInfosValue.name);
    if (!attrA || !attrB)
        return;

    if (isSingleComponent) {
        if (attrA->attrSize() <= attrAInfosValue.component || attrB->attrSize() <= attrBInfosValue.component)
            return;
    }

    auto attrOut = t_attribs.findOrCreate<float>(outputInfosValue.name, attrA->attrSize());
    if (isSingleComponent && attrOut->attrSize() <= outputInfosValue.component)
        return;

    if (!isSingleComponent) {
        if (attrA->attrSize() != attrB->attrSize() || attrB->attrSize() != attrOut->attrSize())
            return;
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

            infos.name = t_name.substr(0, i - 1);
            infos.singleComponent = true;
            return infos;
        }
    }

    infos.name = t_name;
    return infos;
}

} // namespace euclide