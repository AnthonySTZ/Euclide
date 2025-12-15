#include "attribute_create.h"

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
}

std::shared_ptr<Mesh> AttributeCreate::compute(const size_t t_index,
                                               const std::vector<std::shared_ptr<Mesh>>& t_inputs) const {
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>();

    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    const Kind kind = static_cast<Kind>(getField<NodeField<int>>("kind")->getValue());
    const std::string attrName = getField<NodeField<std::string>>("attributeName")->getValue();
    const int attrSize = getField<NodeField<int>>("size")->getValue();

    if (kind == Kind::POINTS) {
        createAttribute(output->pointAttribs, attrName, attrSize);
    } else if (kind == Kind::PRIMITIVES) {
        createAttribute(output->primAttribs, attrName, attrSize);
    }

    return output;
}

void AttributeCreate::createAttribute(AttributeSet& t_attribs, const std::string& t_name, const int t_attrSize) {
    t_attribs.findOrCreate<float>(t_name, t_attrSize);
}

} // namespace euclide