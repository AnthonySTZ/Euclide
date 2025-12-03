#include "interface/engine.h"

#include "gpu/gpu_manager.h"
#include "gpu/gpu_buffer.h"

int main() {
    euclide::GPUManager& manager = euclide::GPUManager::getInstance();

    uint32_t vertexCount = 10;
    VkDeviceSize bufferSize = sizeof(float) * vertexCount;
    uint32_t vertexSize = sizeof(float);
    {
        euclide::GPUBuffer testBuffer{
            manager.getDevice(),
            vertexSize,
            vertexCount,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        };
    }

    return 0;
    euclide::Engine engine{1600, 900, "Test"};

    engine.interface().addNodeGraph();
    engine.interface().addViewport();
    engine.interface().addParameters();

    engine.mainLoop();
    return 0;
}