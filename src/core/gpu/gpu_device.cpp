#include "gpu_device.h"

#include <iostream>

namespace euclide {

GPUDevice::GPUDevice(VkInstance& t_vkInstance) : vkInstance(t_vkInstance) {
    pickPhysicalDevice();
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