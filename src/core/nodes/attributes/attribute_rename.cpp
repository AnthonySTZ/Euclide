#include "attribute_rename.h"

namespace euclide {

AttributeRename::AttributeRename() : Node(1, 1, "AttrRename") {
    auto kindField = std::make_shared<NodeField<int>>(0);
    kindField->setMetadata(NodeFieldMetadata{
        displayName : "Kind",
        is_combo : true,
        choices : std::move(std::vector<std::string>{"points", "primitives"})
    });
    addField("kind", kindField);

    auto nameField = std::make_shared<NodeField<std::string>>("");
    nameField->setMetadata(NodeFieldMetadata{displayName : "Attribute Name"});
    addField("attributeName", nameField);

    auto newNameField = std::make_shared<NodeField<std::string>>("");
    newNameField->setMetadata(NodeFieldMetadata{displayName : "New Name"});
    addField("newName", newNameField);
}

std::shared_ptr<Mesh> AttributeRename::compute(const size_t t_index,
                                               const std::vector<std::shared_ptr<Mesh>>& t_inputs) {
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>();

    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    const Kind kind = static_cast<Kind>(getField<NodeField<int>>("kind")->getValue());
    const std::string name = getField<NodeField<std::string>>("attributeName")->getValue();
    const std::string newName = getField<NodeField<std::string>>("newName")->getValue();

    if (name.empty() || newName.empty())
        return output;

    if (kind == Kind::POINTS) {
        renameAttribute(output->pointAttribs, name, newName);
    } else if (kind == Kind::PRIMITIVES) {
        renameAttribute(output->primAttribs, name, newName);
    }

    return output;
}

void AttributeRename::renameAttribute(AttributeSet& t_attribs, const std::string& t_name,
                                      const std::string& t_newName) {
    t_attribs.rename(t_name, t_newName);
}

} // namespace euclide