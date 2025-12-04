#pragma once

#include "gpu_device.h"

#include <unordered_map>

namespace euclide {

class GPUDescriptorSetLayout {
  public:
    class Builder {
      public:
        Builder(GPUDevice& t_device) : m_device(t_device){};

        Builder& addBinding(uint32_t t_binding, VkDescriptorType t_descriptorType, VkShaderStageFlags t_stageFlags,
                            uint32_t t_count = 1);

      private:
        GPUDevice& m_device;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings{};
    };

  private:
};

} // namespace euclide