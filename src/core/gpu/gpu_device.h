#pragma once

#include <vulkan/vulkan.hpp>

#include <optional>

namespace euclide {

class GPUDevice {
  public:
    GPUDevice(VkInstance& t_vkInstance);
    ~GPUDevice();

    /// @brief Destroy the logical device if exists
    void destroy();

    /// @brief Returns the used physical device
    /// @return `VkPhysicalDevice` choosen based on its properties
    [[nodiscard]] VkPhysicalDevice physicalDevice() const noexcept { return m_physicalDevice; }

    /// @brief Logical device, the 'interface' with the physical device
    /// @return `VkDevice` Logical device
    [[nodiscard]] VkDevice device() const noexcept { return m_device; }

    /// @brief Find a queue family index that match the given flags
    /// @param t_physicalDevice
    /// @param t_flags
    /// @return The Queue family index if found, any otherwise.
    [[nodiscard]] std::optional<uint32_t> findQueueFamilies(const VkPhysicalDevice& t_physicalDevice,
                                                            const VkQueueFlagBits t_flags) const;

    /// @brief Find a queue family index that match the given flags using the current Physical Device
    /// @param t_flags
    /// @return The Queue family index if found, any otherwise.
    [[nodiscard]] std::optional<uint32_t> findPhysicalQueueFamilies(const VkQueueFlagBits t_flags) const {
        return findQueueFamilies(m_physicalDevice, t_flags);
    }

    /// @brief Create logical device for compute shader only
    void createComputeLogicalDevice();

    /// @brief Create buffer based on given parameters, allocate memory and bind it.
    /// @param size
    /// @param usage
    /// @param properties
    /// @param buffer
    /// @param bufferMemory
    void createBuffer(VkDeviceSize t_size, VkBufferUsageFlags t_usage, VkMemoryPropertyFlags t_properties,
                      VkBuffer& t_buffer, VkDeviceMemory& t_bufferMemory) const;

  private:
    void pickPhysicalDevice();

    uint32_t findMemoryType(uint32_t t_typeFilter, VkMemoryPropertyFlags t_properties) const;

  private:
    VkInstance vkInstance = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties m_physicalDeviceProperties{};

    VkDevice m_device = VK_NULL_HANDLE;
};

} // namespace euclide