#pragma once

#include "gpu_device.h"

namespace euclide {

class GPUBuffer {
  public:
    GPUBuffer(GPUDevice& t_device, const VkDeviceSize t_instanceSize, const uint32_t t_instanceCount,
              const VkBufferUsageFlags t_usageFlags, const VkMemoryPropertyFlags t_memoryPropertyFlags,
              const VkDeviceSize t_minOffsetAlignment = 1);
    ~GPUBuffer();

    /// @brief Map current buffer to the specified size and offset
    /// @param size
    /// @param offset
    /// @return VK_SUCCESS if map has been successful
    VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

    /// @brief Unmap current Buffer if mapped
    void unmap();

    /// @brief Copies the specified data to the mapped buffer. Default value writes whole buffer range
    /// @param data Pointer to the data to copy
    /// @param size (Optional) Size of the data to copy. Pass VK_WHOLE_SIZE to flush the complete buffer
    /// range.
    /// @param offset (Optional) Byte offset from beginning of mapped region
    void writeToBuffer(void* t_data, VkDeviceSize t_size = VK_WHOLE_SIZE, VkDeviceSize t_offset = 0);

    [[nodiscard]] VkDescriptorBufferInfo descriptorInfo(VkDeviceSize t_size = VK_WHOLE_SIZE,
                                                        VkDeviceSize t_offset = 0) const;

    /// @brief Returns the current buffer
    /// @return Current `VkBuffer`
    [[nodiscard]] VkBuffer buffer() const noexcept { return m_buffer; }

    [[nodiscard]] void* ptr() const noexcept { return m_mapped; }

  private:
    /// @brief Returns the minimum instance size required to be compatible with devices minOffsetAlignment
    /// @param instanceSize The size of an instance
    /// @param minOffsetAlignment The minimum required alignment, in bytes, for the offset member (eg
    /// minUniformBufferOffsetAlignment)
    /// @return VkResult of the buffer mapping call
    [[nodiscard]] VkDeviceSize getAlignment(VkDeviceSize t_instanceSize, VkDeviceSize t_minOffsetAlignment);

  private:
    GPUDevice& m_device;
    VkBuffer m_buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_memory = VK_NULL_HANDLE;
    void* m_mapped = nullptr;

    VkDeviceSize m_bufferSize;
    VkDeviceSize m_alignmentSize;
    VkDeviceSize m_instanceSize;
};

} // namespace euclide