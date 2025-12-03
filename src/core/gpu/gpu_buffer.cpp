#include "gpu_buffer.h"

namespace euclide {

GPUBuffer::GPUBuffer(GPUDevice& t_device, const VkDeviceSize t_instanceSize, const uint32_t t_instanceCount,
                     const VkBufferUsageFlags t_usageFlags, const VkMemoryPropertyFlags t_memoryPropertyFlags,
                     const VkDeviceSize t_minOffsetAlignment)
    : m_device{t_device}, m_instanceSize{t_instanceSize} {
    m_alignmentSize = getAlignment(t_instanceSize, t_minOffsetAlignment);
    m_bufferSize = m_alignmentSize * t_instanceCount;
    m_device.createBuffer(m_bufferSize, t_usageFlags, t_memoryPropertyFlags, m_buffer, m_memory);
}

GPUBuffer::~GPUBuffer() {
    unmap();
    vkDestroyBuffer(m_device.device(), m_buffer, nullptr);
    vkFreeMemory(m_device.device(), m_memory, nullptr);
}

VkResult GPUBuffer::map(VkDeviceSize size, VkDeviceSize offset) {
    assert(m_buffer && m_memory && "Called map on buffer before create");
    return vkMapMemory(m_device.device(), m_memory, offset, size, 0, &m_mapped);
}

void GPUBuffer::unmap() {
    if (m_mapped) {
        vkUnmapMemory(m_device.device(), m_memory);
        m_mapped = nullptr;
    }
}

VkDeviceSize GPUBuffer::getAlignment(VkDeviceSize t_instanceSize, VkDeviceSize t_minOffsetAlignment) {
    if (t_minOffsetAlignment > 0) {
        return (t_instanceSize + t_minOffsetAlignment - 1) & ~(t_minOffsetAlignment - 1);
    }
    return t_instanceSize;
}

} // namespace euclide