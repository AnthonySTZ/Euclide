#include "gpu_device.h"

#include <iostream>

namespace euclide {

GPUDevice::GPUDevice(VkInstance& t_vkInstance) : vkInstance(t_vkInstance) {
    pickPhysicalDevice();
}

GPUDevice::~GPUDevice() {
    destroy();
}

void GPUDevice::destroy() {
    if (m_device != VK_NULL_HANDLE) {
        vkDestroyDevice(m_device, nullptr);
    }
}

std::optional<uint32_t> GPUDevice::findQueueFamilies(const VkPhysicalDevice& t_physicalDevice,
                                                     const VkQueueFlagBits t_flags) const {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(t_physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(t_physicalDevice, &queueFamilyCount, queueFamilies.data());

    auto propIt = std::find_if(queueFamilies.begin(), queueFamilies.end(),
                               [&](const VkQueueFamilyProperties& t_prop) { return t_prop.queueFlags & t_flags; });
    if (propIt == queueFamilies.end())
        return std::nullopt;

    return std::distance(queueFamilies.begin(), propIt);
}

void GPUDevice::createComputeLogicalDevice() {
    std::optional<uint32_t> familyIndex = findPhysicalQueueFamilies(VK_QUEUE_COMPUTE_BIT);
    if (!familyIndex.has_value()) {
        throw std::runtime_error("failed to find queue family index with compute flag!");
    }

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = familyIndex.value();
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};
    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    deviceCreateInfo.enabledExtensionCount = 0;
    deviceCreateInfo.enabledLayerCount = 0;

    if (vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device!");
    }
}

void GPUDevice::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

    // TODO: Check which device to choose, for now I stick with the first one
    m_physicalDevice = devices[0];

    if (m_physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);
    std::cout << "Current Physical Device used : " << m_physicalDeviceProperties.deviceName << '\n';
}

} // namespace euclide