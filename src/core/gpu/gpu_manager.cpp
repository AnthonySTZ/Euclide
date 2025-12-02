#include "gpu_manager.h"

#include <iostream>

namespace euclide {

GPUManager& GPUManager::getInstance() {
    static GPUManager instance;
    return instance;
}

GPUManager::GPUManager() {
    initVulkan();
    pickPhysicalDevice();
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
    vkInstance = vk::createInstance(InstanceCreateInfo);
}

void GPUManager::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

    // TODO: Check which device to choose, for now I stick with the first one
    physicalDevice = devices[0];

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
    std::cout << "Current Physical Device used : " << physicalDeviceProperties.deviceName << '\n';
}

} // namespace euclide