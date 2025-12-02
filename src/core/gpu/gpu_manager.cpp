#include "gpu_manager.h"

namespace euclide {

GPUManager& GPUManager::getInstance() {
    static GPUManager instance;
    return instance;
}

GPUManager::GPUManager() {
    initVulkan();
    m_device = std::make_unique<GPUDevice>(m_vkInstance);
}

GPUManager::~GPUManager() {
    if (m_vkInstance != VK_NULL_HANDLE) {
        vkDestroyInstance(m_vkInstance, nullptr);
    }
}

void GPUManager::initVulkan() {
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
    m_vkInstance = vk::createInstance(InstanceCreateInfo);
}

} // namespace euclide