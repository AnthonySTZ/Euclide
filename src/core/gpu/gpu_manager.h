#pragma once

#include <vulkan/vulkan.hpp>
#include <memory>

namespace euclide {

class GPUManager {
  public:
    static inline GPUManager& getInstance() {
        static GPUManager instance;
        return instance;
    }

  private:
    GPUManager(){};
    ~GPUManager(){};
    GPUManager(const GPUManager&) = delete;
    GPUManager& operator=(const GPUManager&) = delete;
};

} // namespace euclide