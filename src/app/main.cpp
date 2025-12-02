#include "interface/engine.h"

#include "gpu/gpu_manager.h"

int main() {
    euclide::GPUManager& manager = euclide::GPUManager::getInstance();

    auto family = manager.getDevice().findPhysicalQueueFamilies(VK_QUEUE_COMPUTE_BIT);
    if (family.has_value()) {
        std::cout << family.value() << '\n';
    }

    manager.getDevice().createComputeLogicalDevice();

    return 0;
    euclide::Engine engine{1600, 900, "Test"};

    engine.interface().addNodeGraph();
    engine.interface().addViewport();
    engine.interface().addParameters();

    engine.mainLoop();
    return 0;
}