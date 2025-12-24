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

    auto secondAttribField = std::make_shared<NodeField<std::string>>("");
    secondAttribField->setMetadata(NodeFieldMetadata{
        .displayName = "Attribute B",
    });
    addField("attrB", secondAttribField);

    auto operationField = std::make_shared<NodeField<int>>(0);
    operationField->setMetadata(NodeFieldMetadata{
        displayName : "Operation",
        is_combo : true,
        choices : std::move(std::vector<std::string>{"Add", "Subtract", "Multiply", "Divide"})
    });
    addField("operation", operationField);

    auto outputAttribField = std::make_shared<NodeField<std::string>>("");
    outputAttribField->setMetadata(NodeFieldMetadata{
        .displayName = "Output A",
    });
    addField("outputA", outputAttribField);
}

void AttributeMath::addAttribute(AttributeSet& t_attribs, const std::string& t_firstAttrib,
                                 const std::string& t_secondAttrib, const std::string& t_resultAttrib) {
}

std::shared_ptr<Mesh> AttributeMath::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) {
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>();

    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    return output;
}

} // namespace euclide