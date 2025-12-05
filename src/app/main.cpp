#include "interface/engine.h"

#include "gpu/gpu_manager.h"
#include "gpu/gpu_buffer.h"
#include "gpu/gpu_descriptor.h"
#include "gpu/gpu_pipeline.h"
#include "gpu/gpu_compute_task.h"

#include <numeric>

void testGpu() {
    euclide::GPUManager& manager = euclide::GPUManager::getInstance();

    uint32_t vertexCount = 10;
    std::vector<float> numbers(vertexCount);
    std::iota(numbers.begin(), numbers.end(), 1.0f);
    {
        euclide::GPUBuffer inBuffer =
            euclide::GPUBuffer::create<float>(manager.getDevice(), vertexCount, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
        inBuffer.write(numbers.data());

        euclide::GPUBuffer outBuffer =
            euclide::GPUBuffer::create<float>(manager.getDevice(), vertexCount, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

        auto descriptorsetLayout = euclide::GPUDescriptorSetLayout::Builder(manager.getDevice())
                                       .addBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                       .addBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1)
                                       .build();

        euclide::GPUPipeline pipeline{manager.getDevice(), "gpu/shaders/square.spv", *descriptorsetLayout};
        euclide::GPUComputeTask task{manager.getDevice(), pipeline, {&inBuffer, &outBuffer}};
        task.run(vertexCount);

        std::cout << "In buffer:\n";
        std::vector<float> inResults(vertexCount);
        inBuffer.read(inResults.data());
        for (auto v : inResults)
            std::cout << v << " ";

        std::cout << "\nOut buffer:\n";
        std::vector<float> outResults(vertexCount);
        outBuffer.read(outResults.data());
        for (auto v : outResults)
            std::cout << v << " ";
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