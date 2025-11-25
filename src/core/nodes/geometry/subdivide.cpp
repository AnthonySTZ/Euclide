#include "subdivide.h"

namespace butter {
    
Subdivide::Subdivide()
    : Node(1, 1, "Subdivide")
{
    auto divisionsField = std::make_shared<NodeField<int>>(1);
    divisionsField->setMetadata(NodeFieldMetadata{
        displayName: "Divisions",
        min: 0,
        step: 1,
    });
    addField("divisions", divisionsField);
}

std::shared_ptr<Mesh> Subdivide::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>> &t_inputs) const
{
    if (t_inputs[0] == nullptr) return std::make_shared<Mesh>();

    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    const int divisions = getField<NodeField<int>>("divisions")->getValue();

    const SubdivideSettings settings{
        .divisions = divisions
    };

    subdivide(*output, settings);

    return output;
}

void Subdivide::subdivide(Mesh &t_mesh, const SubdivideSettings& t_settings)
{    
    
}

}