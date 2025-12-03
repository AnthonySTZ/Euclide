#pragma once

#include "gpu_device.h"

namespace euclide {

class GPUPipeline {
  public:
    GPUPipeline(GPUDevice& t_device, const std::string& t_shaderFile);
    ~GPUPipeline();

    void createShaderModule(const std::string& t_shaderFile, VkShaderModule* t_shaderModule);

  private:
    GPUDevice& m_device;
    VkPipeline m_pipeline;
    VkShaderModule m_shaderModule;
};

} // namespace euclide