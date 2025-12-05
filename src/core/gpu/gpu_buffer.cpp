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

void GPUBuffer::writeToBuffer(void* t_data, VkDeviceSize t_size, VkDeviceSize t_offset) {
    assert(m_mapped && "Cannot copy to unmapped buffer");

    if (t_size == VK_WHOLE_SIZE) {
        memcpy(m_mapped, t_data, m_bufferSize);
    } else {
        char* memOffset = (char*)m_mapped;
        memOffset += t_offset;
        memcpy(memOffset, t_data, t_size);
    }
}

void GPUBuffer::write(void* t_data, VkDeviceSize t_size, VkDeviceSize t_offset) {
    map(t_size, t_offset);
    writeToBuffer(t_data);
    unmap();
}

VkDescriptorBufferInfo GPUBuffer::descriptorInfo(VkDeviceSize t_size, VkDeviceSize t_offset) const {
    return VkDescriptorBufferInfo{
        m_buffer,
        t_offset,
        t_size,
    };
}

void GPUBuffer::read(void* t_data) {
    map();
    memcpy(t_data, m_mapped, m_bufferSize);
    unmap();
}

VkDeviceSize GPUBuffer::getAlignment(VkDeviceSize t_instanceSize, VkDeviceSize t_minOffsetAlignment) {
    if (t_minOffsetAlignment > 0) {
        return (t_instanceSize + t_minOffsetAlignment - 1) & ~(t_minOffsetAlignment - 1);
    }
    return t_instanceSize;
}

} // namespace euclide