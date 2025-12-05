#include "gpu_compute_task.h"

namespace euclide {

GPUComputeTask::GPUComputeTask(GPUDevice& t_device, GPUPipeline& t_pipeline,
                               const std::vector<GPUBuffer*>& t_buffersPtr)
    : m_device(t_device), m_pipeline(t_pipeline) {
    m_descriptorPool = GPUDescriptorPool::Builder(t_device)
                           .addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, t_buffersPtr.size())
                           .setMaxSets(1)
                           .setPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
                           .build();

    GPUDescriptorWriter writer{m_pipeline.descriptorSetLayout(), *m_descriptorPool};
    for (size_t binding = 0; binding < t_buffersPtr.size(); ++binding) {
        writer.writeBuffer(binding, t_buffersPtr[binding]->descriptorInfo());
    }
    writer.build(m_descriptorSet);
}

void GPUComputeTask::run(const uint32_t t_x, const uint32_t t_y, const uint32_t t_z) {
    VkCommandBuffer cmdBuffer = m_device.beginSingleTimeCommands();

    m_pipeline.bind(cmdBuffer);
    vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_pipeline.layout(), 0, 1, &m_descriptorSet, 0,
                            nullptr);

    vkCmdDispatch(cmdBuffer, t_x, t_y, t_z);

    m_device.endSingleTimeCommands(cmdBuffer);

    m_descriptorPool->freeDescriptor(m_descriptorSet);
}

} // namespace euclide