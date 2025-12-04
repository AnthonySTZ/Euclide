#pragma once

#include "gpu_device.h"

#include <unordered_map>
#include <memory>

namespace euclide {

class GPUDescriptorSetLayout {
  public:
    class Builder {
      public:
        Builder(GPUDevice& t_device) : m_device(t_device){};

        [[nodiscard]] Builder& addBinding(uint32_t t_binding, VkDescriptorType t_descriptorType,
                                          VkShaderStageFlags t_stageFlags, uint32_t t_count = 1);

        [[nodiscard]] std::unique_ptr<GPUDescriptorSetLayout> build() const;

      private:
        GPUDevice& m_device;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings{};
    };

  public:
    GPUDescriptorSetLayout(GPUDevice& t_device,
                           const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>& t_bindings);
    ~GPUDescriptorSetLayout();
    GPUDescriptorSetLayout(const GPUDescriptorSetLayout&) = delete;
    GPUDescriptorSetLayout& operator=(const GPUDescriptorSetLayout&) = delete;

    [[nodiscard]] VkDescriptorSetLayout descriptorSetLayout() const noexcept { return m_descriptorSetLayout; }

  private:
    GPUDevice& m_device;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings;

    VkDescriptorSetLayout m_descriptorSetLayout;
};

} // namespace euclide