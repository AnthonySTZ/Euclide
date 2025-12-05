#pragma once

#include "gpu_pipeline.h"
#include "gpu_buffer.h"
#include "gpu_descriptor.h"

namespace euclide {
class GPUComputeTask {
  public:
    GPUComputeTask(GPUDevice& t_device, GPUPipeline& t_pipeline, const std::vector<GPUBuffer*>& t_buffersPtr);

    void run(const uint32_t t_x, const uint32_t t_y = 1, const uint32_t t_z = 1);

  private:
    GPUDevice& m_device;
    GPUPipeline& m_pipeline;
    std::unique_ptr<GPUDescriptorPool> m_descriptorPool;
    VkDescriptorSet m_descriptorSet{};
};

} // namespace euclide