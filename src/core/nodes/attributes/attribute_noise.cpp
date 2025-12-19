#include "attribute_noise.h"

namespace euclide {

AttributeNoise::AttributeNoise() : Node(1, 1, "AttrNoise") {
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

    auto noiseField = std::make_shared<NodeField<int>>(0);
    noiseField->setMetadata(NodeFieldMetadata{
        displayName : "Noise",
        is_combo : true,
        choices : std::move(std::vector<std::string>{"Perlin"})
    });
    addField("noise", noiseField);
}

std::shared_ptr<Mesh> AttributeNoise::compute(const size_t t_index,
                                              const std::vector<std::shared_ptr<Mesh>>& t_inputs) const {
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>();

    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    perlinNoise(output->pointAttribs, "Cd", 3, 1, 1);

    return output;
}

void AttributeNoise::perlinNoise(AttributeSet& t_attribs, const std::string& t_name, const int t_attrSize,
                                 const int t_octaves, const float persistence) {
}

} // namespace euclide