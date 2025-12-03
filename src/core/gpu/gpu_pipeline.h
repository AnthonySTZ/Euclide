#pragma once

#include "gpu_device.h"

namespace euclide {

class GPUPipeline {
  public:
    GPUPipeline(GPUDevice& t_device, const std::string& t_shaderFile,
                const std::vector<VkDescriptorSetLayoutBinding>& t_descriptorSetLayoutBindings);
    ~GPUPipeline();

  private:
    void createShaderModule(const std::string& t_shaderFile, VkShaderModule* t_shaderModule);
    void createDescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& t_descriptorSetLayoutBindings);

    void createPipeline();

  private:
    GPUDevice& m_device;
    VkPipelineLayout m_pipelineLayout;
    VkPipelineCache m_pipelineCache;
    VkPipeline m_pipeline;
    VkShaderModule m_shaderModule;
    VkDescriptorSetLayout m_descriptorSetLayout;
};

} // namespace euclide