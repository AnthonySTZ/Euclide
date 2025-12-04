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

class GPUDescriptorPool {
  public:
    class Builder {
      public:
        Builder(GPUDevice& t_device) : m_device(t_device) {}

        [[nodiscard]] Builder& addPoolSize(VkDescriptorType t_descriptorType, uint32_t t_count);
        [[nodiscard]] Builder& setPoolFlags(VkDescriptorPoolCreateFlags t_flags);
        [[nodiscard]] Builder& setMaxSets(uint32_t t_count);
        [[nodiscard]] std::unique_ptr<GPUDescriptorPool> build() const;

      private:
        GPUDevice& m_device;
        std::vector<VkDescriptorPoolSize> m_poolSizes{};
        uint32_t m_maxSets = 1000;
        VkDescriptorPoolCreateFlags m_poolFlags = 0;
    };

  public:
    GPUDescriptorPool(GPUDevice& t_device, uint32_t t_maxSets, VkDescriptorPoolCreateFlags t_poolFlags,
                      const std::vector<VkDescriptorPoolSize>& t_poolSizes);
    ~GPUDescriptorPool();
    GPUDescriptorPool(const GPUDescriptorPool&) = delete;
    GPUDescriptorPool& operator=(const GPUDescriptorPool&) = delete;

  private:
    GPUDevice& m_device;
    VkDescriptorPool m_descriptorPool;
};

} // namespace euclide