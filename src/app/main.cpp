#include "interface/engine.h"

#include "gpu/gpu_manager.h"
#include "gpu/gpu_buffer.h"
#include "gpu/gpu_descriptor.h"
#include "gpu/gpu_pipeline.h"

#include <numeric>

void testGpu() {
    euclide::GPUManager& manager = euclide::GPUManager::getInstance();

    uint32_t vertexCount = 10;
    uint32_t vertexSize = sizeof(float);
    std::vector<float> numbers(vertexCount);
    std::iota(numbers.begin(), numbers.end(), 1.0f);
    {
        euclide::GPUBuffer inBuffer{
            manager.getDevice(),
            vertexSize,
            vertexCount,
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        };

        inBuffer.map();
        inBuffer.writeToBuffer(numbers.data());
        inBuffer.unmap();

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
                                  .setMaxSets(1)
                                  .setPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
                                  .build();

        VkDescriptorSet descriptorSet{};

        euclide::GPUDescriptorWriter(*descriptorsetLayout, *descriptorPool)
            .writeBuffer(0, inBuffer.descriptorInfo())
            .writeBuffer(1, outBuffer.descriptorInfo())
            .build(descriptorSet);

        manager.getDevice().createCommandPool();
        VkCommandBuffer cmdBuffer = manager.getDevice().beginSingleTimeCommands();

        pipeline.bind(cmdBuffer);
        vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline.layout(), 0, 1, &descriptorSet, 0,
                                nullptr);

        vkCmdDispatch(cmdBuffer, vertexCount, 1, 1);

        manager.getDevice().endSingleTimeCommands(cmdBuffer);

        descriptorPool->freeDescriptor(descriptorSet);

        std::cout << "In buffer:\n";
        inBuffer.map();
        float* inBufferPtr = static_cast<float*>(inBuffer.ptr());
        for (size_t i = 0; i < vertexCount; ++i) {
            std::cout << inBufferPtr[i] << " ";
        }
        inBuffer.unmap();

        std::cout << "\nOut buffer:\n";
        outBuffer.map();
        float* OutBufferPtr = static_cast<float*>(outBuffer.ptr());
        for (size_t i = 0; i < vertexCount; ++i) {
            std::cout << OutBufferPtr[i] << " ";
        }
        outBuffer.unmap();
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