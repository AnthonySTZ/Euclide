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
}

std::shared_ptr<Mesh> AttributeCreate::compute(const size_t t_index,
                                               const std::vector<std::shared_ptr<Mesh>>& t_inputs) const {
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>();

    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    return output;
}

} // namespace euclide