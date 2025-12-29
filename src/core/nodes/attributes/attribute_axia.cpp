#include "attribute_axia.h"

#include "fields/float4field.h"
#include "axia/parser.h"

namespace euclide {

AttributeAXIA::AttributeAXIA() : Node(1, 1, "Axia") {
    auto kindField = std::make_shared<NodeField<int>>(0);
    kindField->setMetadata(NodeFieldMetadata{
        displayName : "Kind",
        isCombo : true,
        choices : std::move(std::vector<std::string>{"points", "primitives"})
    });
    addField("kind", kindField);

    auto scriptField = std::make_shared<NodeField<std::string>>("");
    scriptField->setMetadata(NodeFieldMetadata{
        .displayName = "AXIA Script",
        .isMultiline = true,
    });
    addField("script", scriptField);
}

std::shared_ptr<Mesh> AttributeAXIA::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) {
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>();

    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    const Kind kind = static_cast<Kind>(getField<NodeField<int>>("kind")->getValue());
    const std::string script = getField<NodeField<std::string>>("script")->getValue();

    if (script.empty())
        return output;

    if (kind == Kind::POINTS) {
        computeAXIA(output->pointAttribs, script);
    } else if (kind == Kind::PRIMITIVES) {
        computeAXIA(output->primAttribs, script);
    }

    return output;
}

void AttributeAXIA::computeAXIA(AttributeSet& t_attribs, const std::string& t_script) {
    Parser parser{};
    const std::vector<AST> parsed = parser.parse(t_script);
}

} // namespace euclide