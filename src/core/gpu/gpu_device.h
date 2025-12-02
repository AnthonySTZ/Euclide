#pragma once

#include <vulkan/vulkan.hpp>

namespace euclide {

class GPUDevice {
  public:
    GPUDevice(VkInstance& t_vkInstance);
    ~GPUDevice() = default;

    [[nodiscard]] VkPhysicalDevice physicalDevice() const noexcept { return m_physicalDevice; }

  private:
    void pickPhysicalDevice();

  private:
    VkInstance vkInstance;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties m_physicalDeviceProperties;
};

} // namespace euclide