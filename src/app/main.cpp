#include "interface/engine.h"

#include "gpu/gpu_manager.h"
#include "gpu/gpu_buffer.h"
#include "gpu/gpu_pipeline.h"

#include <numeric>

void testGpu() {
    euclide::GPUManager& manager = euclide::GPUManager::getInstance();

    uint32_t vertexCount = 10;
    VkDeviceSize bufferSize = sizeof(float) * vertexCount;
    uint32_t vertexSize = sizeof(float);
    {
        euclide::GPUBuffer inBuffer{
            manager.getDevice(),
            vertexSize,
            vertexCount,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        };

        std::vector<float> numbers(10);
        std::iota(numbers.begin(), numbers.end(), 1.0f);
        inBuffer.map();
        inBuffer.writeToBuffer(numbers.data()); // maybe (void*)numbers.data()

        euclide::GPUBuffer outBuffer{
            manager.getDevice(),
            vertexSize,
            vertexCount,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        };

        std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBinding(2);
        descriptorSetLayoutBinding[0].binding = 0;
        descriptorSetLayoutBinding[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptorSetLayoutBinding[0].descriptorCount = 1;
        descriptorSetLayoutBinding[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

        descriptorSetLayoutBinding[1].binding = 1;
        descriptorSetLayoutBinding[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptorSetLayoutBinding[1].descriptorCount = 1;
        descriptorSetLayoutBinding[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

        euclide::GPUPipeline pipeline{manager.getDevice(), "gpu/shaders/square.spv", descriptorSetLayoutBinding};
    }
}

int main() {
    testGpu();
    return 0;
    euclide::Engine engine{1600, 900, "Test"};

    engine.interface().addNodeGraph();
    engine.interface().addViewport();
    engine.interface().addParameters();

    engine.mainLoop();
    return 0;
}