#pragma once

#include "gpu_device.h"
#include "gpu_descriptor.h"

namespace euclide {

class GPUPipeline {
  public:
    GPUPipeline(GPUDevice& t_device, const std::string& t_shaderFile, GPUDescriptorSetLayout& t_descriptorSetLayout);
    ~GPUPipeline();

    [[nodiscard]] VkPipelineLayout layout() const noexcept { return m_pipelineLayout; }

    void bind(VkCommandBuffer t_commandBuffer) const;
    GPUDescriptorSetLayout& descriptorSetLayout() noexcept { return m_descriptorSetLayout; };

  private:
    void createShaderModule(const std::string& t_shaderFile, VkShaderModule* t_shaderModule);
    void createPipelineLayout();

    void createPipeline();

  private:
    GPUDevice& m_device;
    GPUDescriptorSetLayout& m_descriptorSetLayout;
    VkPipelineLayout m_pipelineLayout;
    VkPipelineCache m_pipelineCache;
    VkPipeline m_pipeline;
    VkShaderModule m_shaderModule;
};

} // namespace euclide