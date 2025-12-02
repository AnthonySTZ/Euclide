#pragma once

#include "gpu_device.h"
#include <memory>

namespace euclide {

class GPUManager {
  public:
    /// @brief Get the global `GPUManager` instance to avoid recreating a VkInstance or pass it for all nodes,...
    /// @return `GPUManager` global instance as a reference
    static GPUManager& getInstance();

    /// @brief Get the `VkInstance` created at GPUManager initialization
    /// @return `VkInstance` of the application
    [[nodiscard]] VkInstance getInstanceHandle() const noexcept { return m_vkInstance; }

    /// @brief Get the `GPUDevice` that handles all device operations
    /// @return `GPUDevice` reference
    [[nodiscard]] GPUDevice& getDevice() noexcept { return *m_device; }

  private:
    GPUManager();
    ~GPUManager();

    GPUManager(const GPUManager&) = delete;
    GPUManager& operator=(const GPUManager&) = delete;

    /// @brief Init vulkan instance, called in constructor
    void initVulkan();

  private:
    VkInstance m_vkInstance = VK_NULL_HANDLE;
    std::unique_ptr<GPUDevice> m_device;
};

} // namespace euclide