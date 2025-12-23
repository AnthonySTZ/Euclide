#include "attribute_noise.h"

#include "fields/float3field.h"

namespace euclide {

const std::array<int, 512> PerlinNoise::perlinPermutations = {
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

    auto frequencyField = std::make_shared<Float3Field>(1.0f, 1.0f, 1.0f);
    frequencyField->setMetadata(NodeFieldMetadata{
        displayName : "Frequency",
        min : 0,
        step : 0.01,
    });
    addField("frequency", frequencyField);
    // TODO: add frequency float3
}

std::shared_ptr<Mesh> AttributeNoise::compute(const size_t t_index,
                                              const std::vector<std::shared_ptr<Mesh>>& t_inputs) {
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>();

    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    const int attrSize = getField<NodeField<int>>("size")->getValue();
    if (attrSize < 1)
        return output;

    const Kind kind = static_cast<Kind>(getField<NodeField<int>>("kind")->getValue());
    const std::string attrName = getField<NodeField<std::string>>("attributeName")->getValue();
    const int octaves = getField<NodeField<int>>("octaves")->getValue();
    const float3 frequency = getField<Float3Field>("frequency")->getValue();

    if (attrName.empty())
        return output;

    if (kind == Kind::POINTS) {
        m_perlinNoise.applyToMesh(*output, output->pointAttribs, attrName, attrSize,
                                  PerlinNoise::PerlinSettings{octaves, frequency});
    } else if (kind == Kind::PRIMITIVES) {
        m_perlinNoise.applyToMesh(*output, output->primAttribs, attrName, attrSize,
                                  PerlinNoise::PerlinSettings{octaves, frequency});
    }

    return output;
}

PerlinNoise::PerlinNoise()
    : m_device(GPUManager::getInstance().getDevice()),
      m_descriptorSetLayout(GPUDescriptorSetLayout::Builder(m_device)
                                .addBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                .addBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                .addBinding(2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                .addBinding(3, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                .addBinding(4, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                .addBinding(5, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                .build()),
      m_pipeline(GPUPipeline{m_device, "gpu/shaders/perlin.spv", *m_descriptorSetLayout}),
      m_inBufPosX(GPUBuffer::create<float>(m_device, 1, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)),
      m_inBufPosY(GPUBuffer::create<float>(m_device, 1, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)),
      m_inBufPosZ(GPUBuffer::create<float>(m_device, 1, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)),
      m_inBufPermutations(GPUBuffer::create<int>(m_device, 512, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)),
      m_inBufParams(GPUBuffer::create<PerlinNoise::BufferParams>(m_device, 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)),
      m_outBuf(GPUBuffer::create<float>(m_device, 1, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)) {
    m_inBufPermutations.write(perlinPermutations.data());
}

void PerlinNoise::applyToMesh(Mesh& t_mesh, AttributeSet& t_attribs, const std::string& t_name, const int t_attrSize,
                              const PerlinNoise::PerlinSettings& t_settings) {
    const int numPoints = t_attribs.size();
    const PerlinNoise::BufferParams perlinParams{t_settings.frequency, 0, t_settings.octaves, numPoints};

    auto positions = t_mesh.pointAttribs.find("P");
    const float* posX = positions->component<float>(0);
    const float* posY = positions->component<float>(1);
    const float* posZ = positions->component<float>(2);

    m_inBufPosX.ensureSize(numPoints);
    m_inBufPosX.write(posX);

    m_inBufPosY.ensureSize(numPoints);
    m_inBufPosY.write(posY);

    m_inBufPosZ.ensureSize(numPoints);
    m_inBufPosZ.write(posZ);

    m_inBufParams.write(&perlinParams);

    m_outBuf.ensureSize(numPoints);

    GPUComputeTask task{
        m_device,
        m_pipeline,
        {
            &m_inBufPosX,
            &m_inBufPosY,
            &m_inBufPosZ,
            &m_outBuf,
            &m_inBufPermutations,
            &m_inBufParams,
        },
        STORAGE_BUFFER_COUNT,
        UNIFORM_BUFFER_COUNT,
    };
    size_t groupCount = (numPoints + 255) / 256;
    task.run(groupCount);

    auto attr = t_attribs.findOrCreate<float>(t_name, t_attrSize);
    const size_t attrSize = attr->attrSize();
    float* dataPtr = attr->component<float>(0);
    m_outBuf.read(dataPtr);

    float dataSize = attr->size() * sizeof(float);
    for (size_t c = 1; c < attrSize; ++c) {
        memcpy(attr->component<float>(c), dataPtr, dataSize);
    }
}

} // namespace euclide