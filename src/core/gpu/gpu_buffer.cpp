#include "gpu_buffer.h"

namespace euclide {

GPUBuffer::GPUBuffer(GPUDevice& t_device, const VkDeviceSize t_instanceSize, const uint32_t t_instanceCount,
                     const VkBufferUsageFlags t_usageFlags, const VkMemoryPropertyFlags t_memoryPropertyFlags,
                     const VkDeviceSize t_minOffsetAlignment)
    : m_device(t_device), m_instanceSize(t_instanceSize), m_usageFlags(t_usageFlags),
      m_memoryPropertyFlags(t_memoryPropertyFlags) {
    m_alignmentSize = getAlignment(t_instanceSize, t_minOffsetAlignment);
    m_bufferSize = m_alignmentSize * t_instanceCount;

    m_device.createBuffer(m_bufferSize, m_usageFlags, m_memoryPropertyFlags, m_buffer, m_memory);
}

GPUBuffer::~GPUBuffer() {
    free();
}

GPUBuffer::GPUBuffer(GPUDevice& t_device) : m_device(t_device) {
}

GPUBuffer::GPUBuffer(GPUBuffer&& t_other)
    : m_device(t_other.m_device), m_buffer(t_other.m_buffer), m_memory(t_other.m_memory), m_mapped(t_other.m_mapped),
      m_bufferSize(t_other.m_bufferSize), m_alignmentSize(t_other.m_alignmentSize),
      m_instanceSize(t_other.m_instanceSize), m_usageFlags(t_other.m_usageFlags),
      m_memoryPropertyFlags(t_other.m_memoryPropertyFlags) {
    t_other.m_buffer = VK_NULL_HANDLE;
    t_other.m_memory = VK_NULL_HANDLE;
    t_other.m_mapped = nullptr;
    t_other.m_bufferSize = 0;
    t_other.m_alignmentSize = 0;
}

GPUBuffer& GPUBuffer::operator=(GPUBuffer&& t_other) {
    free();
    m_device = t_other.m_device;
    m_buffer = t_other.m_buffer;
    m_memory = t_other.m_memory;
    m_mapped = t_other.m_mapped;
    m_bufferSize = t_other.m_bufferSize;
    m_alignmentSize = t_other.m_alignmentSize;
    m_instanceSize = t_other.m_instanceSize;
    m_usageFlags = t_other.m_usageFlags;
    m_memoryPropertyFlags = t_other.m_memoryPropertyFlags;

    t_other.m_buffer = VK_NULL_HANDLE;
    t_other.m_memory = VK_NULL_HANDLE;
    t_other.m_mapped = nullptr;
    t_other.m_bufferSize = 0;
    t_other.m_alignmentSize = 0;

    return *this;
}

void GPUBuffer::free() {
    unmap();
    if (m_buffer)
        vkDestroyBuffer(m_device.device(), m_buffer, nullptr);
    if (m_memory)
        vkFreeMemory(m_device.device(), m_memory, nullptr);

    m_buffer = VK_NULL_HANDLE;
    m_memory = VK_NULL_HANDLE;
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

void GPUBuffer::writeToBuffer(const void* t_data, VkDeviceSize t_size, VkDeviceSize t_offset) {
    assert(m_mapped && "Cannot copy to unmapped buffer");

    if (t_size == VK_WHOLE_SIZE) {
        memcpy(m_mapped, t_data, m_bufferSize);
    } else {
        char* memOffset = (char*)m_mapped;
        memOffset += t_offset;
        memcpy(memOffset, t_data, t_size);
    }
}

void GPUBuffer::write(const void* t_data, VkDeviceSize t_size, VkDeviceSize t_offset) {
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

void GPUBuffer::ensureSize(const VkDeviceSize t_instanceCount) {
    VkDeviceSize oldSize = m_bufferSize;
    m_bufferSize = m_alignmentSize * t_instanceCount;
    if (oldSize >= m_bufferSize)
        return;

    free();

    m_device.createBuffer(m_bufferSize, m_usageFlags, m_memoryPropertyFlags, m_buffer, m_memory);
}

VkDeviceSize GPUBuffer::getAlignment(VkDeviceSize t_instanceSize, VkDeviceSize t_minOffsetAlignment) {
    if (t_minOffsetAlignment > 0) {
        return (t_instanceSize + t_minOffsetAlignment - 1) & ~(t_minOffsetAlignment - 1);
    }
    return t_instanceSize;
}

} // namespace euclide