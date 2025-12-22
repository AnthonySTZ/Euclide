#include "attribute_noise.h"

namespace euclide {

const std::array<int, 512> AttributeNoise::perlinPermutations = {
    151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225, 140, 36,  103, 30,  69,  142, 8,
    99,  37,  240, 21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117, 35,
    11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136, 171, 168, 68,  175, 74,  165, 71,  134, 139,
    48,  27,  166, 77,  146, 158, 231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,  245, 40,
    244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169, 200, 196, 135,
    130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,   202, 38,  147,
    118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119,
    248, 152, 2,   44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253, 19,  98,  108, 110,
    79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162,
    241, 81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184, 84,  204, 176, 115, 121,
    50,  45,  127, 4,   150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215,
    61,  156, 180, 151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225, 140, 36,  103, 30,
    69,  142, 8,   99,  37,  240, 21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219,
    203, 117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136, 171, 168, 68,  175, 74,  165,
    71,  134, 139, 48,  27,  166, 77,  146, 158, 231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,
    46,  245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169,
    200, 196, 135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,
    202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,  182, 189, 28,  42,  223, 183,
    170, 213, 119, 248, 152, 2,   44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253, 19,
    98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144, 12,
    191, 179, 162, 241, 81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184, 84,  204,
    176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195,
    78,  66,  215, 61,  156, 180};

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

    auto sizeField = std::make_shared<NodeField<int>>(0);
    sizeField->setMetadata(NodeFieldMetadata{
        displayName : "Attribute Size",
        min : 0,
        max : 4,
        step : 1,
    });
    addField("size", sizeField);

    auto noiseField = std::make_shared<NodeField<int>>(0);
    noiseField->setMetadata(NodeFieldMetadata{
        displayName : "Noise",
        is_combo : true,
        choices : std::move(std::vector<std::string>{"Perlin"})
    });
    addField("noise", noiseField);

    auto octavesField = std::make_shared<NodeField<int>>(1);
    octavesField->setMetadata(NodeFieldMetadata{
        displayName : "Octaves",
        min : 1,
        step : 1,
    });
    addField("octaves", octavesField);

    auto frequencyField = std::make_shared<NodeField<float>>(1.0f);
    frequencyField->setMetadata(NodeFieldMetadata{
        displayName : "Frequency",
        min : 0,
        step : 0.01,
    });
    addField("frequency", frequencyField);
    // TODO: add frequency float3
}

std::shared_ptr<Mesh> AttributeNoise::compute(const size_t t_index,
                                              const std::vector<std::shared_ptr<Mesh>>& t_inputs) const {
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>();

    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    const int attrSize = getField<NodeField<int>>("size")->getValue();
    if (attrSize < 1)
        return output;

    const Kind kind = static_cast<Kind>(getField<NodeField<int>>("kind")->getValue());
    const std::string attrName = getField<NodeField<std::string>>("attributeName")->getValue();
    const int octaves = getField<NodeField<int>>("octaves")->getValue();
    const float frequency = getField<NodeField<float>>("frequency")->getValue();

    if (attrName.empty())
        return output;

    if (kind == Kind::POINTS) {
        perlinNoise(*output, output->pointAttribs, attrName, attrSize, PerlinNoiseSettings{octaves, frequency});
    } else if (kind == Kind::PRIMITIVES) {
        perlinNoise(*output, output->primAttribs, attrName, attrSize, PerlinNoiseSettings{octaves, frequency});
    }

    return output;
}

void AttributeNoise::perlinNoise(Mesh& t_mesh, AttributeSet& t_attribs, const std::string& t_name, const int t_attrSize,
                                 const PerlinNoiseSettings& t_settings) {
    const int numPoints = t_attribs.size();
    const PerlinParams perlinParams{numPoints, t_settings.octaves, t_settings.frequency};

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
    GPUBuffer inBufPermutations = GPUBuffer::create<int>(device, 512, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    inBufPermutations.write(perlinPermutations.data());

    GPUBuffer inBufParams = GPUBuffer::create<PerlinParams>(device, 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    inBufParams.write(&perlinParams);

    GPUBuffer outBuffer = GPUBuffer::create<float>(device, numPoints, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

    auto descriptorsetLayout = GPUDescriptorSetLayout::Builder(device)
                                   .addBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                   .addBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                   .addBinding(2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                   .addBinding(3, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                   .addBinding(4, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                   .addBinding(5, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                   .build();

    GPUPipeline pipeline{device, "gpu/shaders/perlin.spv", *descriptorsetLayout};
    GPUComputeTask task{device,
                        pipeline,
                        {
                            &inBufPosX,
                            &inBufPosY,
                            &inBufPosZ,
                            &outBuffer,
                            &inBufPermutations,
                            &inBufParams,
                        }};
    size_t groupCount = (numPoints + 255) / 256;
    task.run(groupCount);

    auto attr = t_attribs.findOrCreate<float>(t_name, t_attrSize);
    const size_t attrSize = attr->attrSize();
    for (size_t c = 0; c < attrSize; ++c) {
        outBuffer.read(attr->component<float>(c));
    }
}

} // namespace euclide