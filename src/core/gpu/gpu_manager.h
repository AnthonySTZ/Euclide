#pragma once

#include <vulkan/vulkan.hpp>
#include <memory>

namespace euclide {

class GPUManager {
  public:
    /// @brief Get the global GPUManager instance to avoid recreating a VkInstance or pass it for all nodes,...
    /// @return GPUManager global instance as a reference
    static inline GPUManager& getInstance();

    VkInstance vkInstance;

  private:
    GPUManager();
    ~GPUManager(){};
    GPUManager(const GPUManager&) = delete;
    GPUManager& operator=(const GPUManager&) = delete;
};

} // namespace euclide