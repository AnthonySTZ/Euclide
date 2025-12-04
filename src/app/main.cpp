#include "interface/engine.h"

#include "gpu/gpu_manager.h"
#include "gpu/gpu_buffer.h"
#include "gpu/gpu_descriptor.h"
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
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
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
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        };

        auto descriptorsetLayout = euclide::GPUDescriptorSetLayout::Builder(manager.getDevice())
                                       .addBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                       .addBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                       .build();

        euclide::GPUPipeline pipeline{manager.getDevice(), "gpu/shaders/square.spv",
                                      descriptorsetLayout->descriptorSetLayout()};

        auto descriptorPool = euclide::GPUDescriptorPool::Builder(manager.getDevice())
                                  .addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2)
                                  .setMaxSets(2)
                                  .setPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
                                  .build();

        VkDescriptorSet descriptorSet{};

        euclide::GPUDescriptorWriter(*descriptorsetLayout, *descriptorPool)
            .writeBuffer(0, inBuffer.descriptorInfo())
            .build(descriptorSet);

        descriptorPool->freeDescriptor(descriptorSet);
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