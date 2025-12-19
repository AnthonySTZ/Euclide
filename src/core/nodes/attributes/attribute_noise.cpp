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

    perlinNoise(*output, output->pointAttribs, "Cd", 3, 1, 1);

    return output;
}

void AttributeNoise::perlinNoise(Mesh& t_mesh, AttributeSet& t_attribs, const std::string& t_name, const int t_attrSize,
                                 const int t_octaves, const float persistence) {
    const size_t numPoints = t_attribs.size();

    auto positions = t_mesh.pointAttribs.find("P");
    const float* posX = positions->component<float>(0);
    const float* posY = positions->component<float>(1);
    const float* posZ = positions->component<float>(2);

    GPUDevice& device = GPUManager::getInstance().getDevice();
    GPUBuffer inBufPosX = GPUBuffer::create<float>(device, numPoints, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    inBufPosX.write(posX);
    GPUBuffer inBufPosY = GPUBuffer::create<float>(device, numPoints, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    inBufPosY.write(posY);
    GPUBuffer inBufPosZ = GPUBuffer::create<float>(device, numPoints, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    inBufPosZ.write(posZ);

    GPUBuffer outBuffer = GPUBuffer::create<float>(device, numPoints, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

    auto descriptorsetLayout = GPUDescriptorSetLayout::Builder(device)
                                   .addBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                   .addBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                   .addBinding(2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                   .addBinding(3, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                   .build();

    GPUPipeline pipeline{device, "gpu/shaders/perlin.spv", *descriptorsetLayout};
    GPUComputeTask task{device,
                        pipeline,
                        {
                            &inBufPosX,
                            &inBufPosY,
                            &inBufPosZ,
                            &outBuffer,
                        }};
    task.run(numPoints);

    auto attr = t_attribs.findOrCreate<float>(t_name, t_attrSize);
    const size_t attrSize = attr->attrSize();
    for (size_t c = 0; c < attrSize; ++c) {
        outBuffer.read(attr->component<float>(c));
    }
}

} // namespace euclide