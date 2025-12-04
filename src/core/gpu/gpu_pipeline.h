#pragma once

#include "gpu_device.h"

namespace euclide {

class GPUPipeline {
  public:
    GPUPipeline(GPUDevice& t_device, const std::string& t_shaderFile,
                const VkDescriptorSetLayout t_descriptorSetLayout);
    ~GPUPipeline();

    [[nodiscard]] VkPipelineLayout layout() const noexcept { return m_pipelineLayout; }

    void bind(VkCommandBuffer t_commandBuffer) const;

  private:
    void createShaderModule(const std::string& t_shaderFile, VkShaderModule* t_shaderModule);
    void createPipelineLayout(const VkDescriptorSetLayout t_descriptorSetLayout);

    void createPipeline();

  private:
    GPUDevice& m_device;
    VkPipelineLayout m_pipelineLayout;
    VkPipelineCache m_pipelineCache;
    VkPipeline m_pipeline;
    VkShaderModule m_shaderModule;
};

} // namespace euclide