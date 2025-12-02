#include "interface/engine.h"

#include "gpu/gpu_manager.h"

int main() {
    euclide::GPUManager& manager = euclide::GPUManager::getInstance();
    return 0;
    euclide::Engine engine{1600, 900, "Test"};

    engine.interface().addNodeGraph();
    engine.interface().addViewport();
    engine.interface().addParameters();

    engine.mainLoop();
    return 0;
}