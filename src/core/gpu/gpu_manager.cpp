#include "gpu_manager.h"

namespace euclide {

inline GPUManager& GPUManager::getInstance() {
    static GPUManager instance;
    return instance;
}

GPUManager::GPUManager() {
    vk::ApplicationInfo AppInfo{
        "Euclide",         // Application Name
        1,                 // Application Version
        nullptr,           // Engine Name or nullptr
        0,                 // Engine Version
        VK_API_VERSION_1_1 // Vulkan API version
    };

    const std::vector<const char*> Layers = {"VK_LAYER_KHRONOS_validation"};
    vk::InstanceCreateInfo InstanceCreateInfo(vk::InstanceCreateFlags(), // Flags
                                              &AppInfo,                  // Application Info
                                              Layers,                    // Layers
                                              {});                       // Extensions
    vkInstance = vk::createInstance(InstanceCreateInfo);
}

} // namespace euclide