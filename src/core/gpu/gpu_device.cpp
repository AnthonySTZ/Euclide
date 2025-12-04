#include "gpu_device.h"

#include <iostream>

namespace euclide {

GPUDevice::GPUDevice(VkInstance& t_vkInstance) : vkInstance(t_vkInstance) {
    pickPhysicalDevice();
    createComputeLogicalDevice();
}

void GPUDevice::destroy() {
    if (m_device != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(m_device);
    }
    if (m_commandPool != VK_NULL_HANDLE) {
        vkResetCommandPool(m_device, m_commandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
        vkDestroyCommandPool(m_device, m_commandPool, nullptr);
    }
    if (m_fence != VK_NULL_HANDLE) {
        vkDestroyFence(m_device, m_fence, nullptr);
    }
    if (m_device != VK_NULL_HANDLE) {
        vkDestroyDevice(m_device, nullptr);
    }
}

std::optional<uint32_t> GPUDevice::findQueueFamilies(const VkPhysicalDevice& t_physicalDevice,
                                                     const VkQueueFlagBits t_flags) const {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(t_physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(t_physicalDevice, &queueFamilyCount, queueFamilies.data());

    auto propIt = std::find_if(queueFamilies.begin(), queueFamilies.end(),
                               [&](const VkQueueFamilyProperties& t_prop) { return t_prop.queueFlags & t_flags; });
    if (propIt == queueFamilies.end())
        return std::nullopt;

    return std::distance(queueFamilies.begin(), propIt);
}

void GPUDevice::createComputeLogicalDevice() {
    std::optional<uint32_t> familyIndex = findPhysicalQueueFamilies(VK_QUEUE_COMPUTE_BIT);
    if (!familyIndex.has_value()) {
        throw std::runtime_error("failed to find queue family index with compute flag!");
    }

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = familyIndex.value();
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};
    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    deviceCreateInfo.enabledExtensionCount = 0;
    deviceCreateInfo.enabledLayerCount = 0;

    if (vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device!");
    }

    vkGetDeviceQueue(m_device, familyIndex.value(), 0, &m_queue);

    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    vkCreateFence(m_device, &fenceCreateInfo, nullptr, &m_fence);
}

void GPUDevice::createBuffer(VkDeviceSize t_size, VkBufferUsageFlags t_usage, VkMemoryPropertyFlags t_properties,
                             VkBuffer& t_buffer, VkDeviceMemory& t_bufferMemory) const {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = t_size;
    bufferInfo.usage = t_usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(m_device, &bufferInfo, nullptr, &t_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_device, t_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, t_properties);

    if (vkAllocateMemory(m_device, &allocInfo, nullptr, &t_bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(m_device, t_buffer, t_bufferMemory, 0);
}

void GPUDevice::createCommandPool() {
    createCommandPool(&m_commandPool);
}

void GPUDevice::createCommandPool(VkCommandPool* t_commandPool) {
    std::optional<uint32_t> queueFamilyIndices = findQueueFamilies(m_physicalDevice, VK_QUEUE_COMPUTE_BIT);
    if (!queueFamilyIndices.has_value()) {
        throw std::runtime_error("failed to find queue families!");
    }

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.value();

    if (vkCreateCommandPool(m_device, &poolInfo, nullptr, t_commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

void GPUDevice::cleanupCommandPool() const {
    vkDestroyCommandPool(m_device, m_commandPool, nullptr);
}

VkCommandBuffer GPUDevice::beginSingleTimeCommands() const {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    return commandBuffer;
}

void GPUDevice::endSingleTimeCommands(VkCommandBuffer t_commandBuffer) const {
    if (vkEndCommandBuffer(t_commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &t_commandBuffer;
    if (vkQueueSubmit(m_queue, 1, &submitInfo, m_fence) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit queue!");
    }
    if (vkWaitForFences(m_device, 1, &m_fence, true, uint64_t(-1)) != VK_SUCCESS) {
        throw std::runtime_error("failed to wait for fence!");
    }

    vkFreeCommandBuffers(m_device, m_commandPool, 1, &t_commandBuffer);
}

void GPUDevice::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

    // TODO: Check which device to choose, for now I stick with the first one
    m_physicalDevice = devices[0];

    if (m_physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);
    std::cout << "Current Physical Device used : " << m_physicalDeviceProperties.deviceName << '\n';
}

uint32_t GPUDevice::findMemoryType(uint32_t t_typeFilter, VkMemoryPropertyFlags t_properties) const {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((t_typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & t_properties) == t_properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

} // namespace euclide