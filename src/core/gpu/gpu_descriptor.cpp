#include "gpu_descriptor.h"

namespace euclide {

GPUDescriptorSetLayout::Builder& GPUDescriptorSetLayout::Builder::addBinding(uint32_t t_binding,
                                                                             VkDescriptorType t_descriptorType,
                                                                             VkShaderStageFlags t_stageFlags,
                                                                             uint32_t t_count) {
    assert(m_bindings.count(t_binding) == 0 && "Binding already in use");
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = t_binding;
    layoutBinding.descriptorType = t_descriptorType;
    layoutBinding.descriptorCount = t_count;
    layoutBinding.stageFlags = t_stageFlags;
    m_bindings[t_binding] = layoutBinding;
    return *this;
}

std::unique_ptr<GPUDescriptorSetLayout> GPUDescriptorSetLayout::Builder::build() const {
    return std::make_unique<GPUDescriptorSetLayout>(m_device, m_bindings);
}

GPUDescriptorSetLayout::GPUDescriptorSetLayout(
    GPUDevice& t_device, const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>& t_bindings)
    : m_device(t_device), m_bindings(t_bindings) {
    std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings;
    for (auto const [_, binding] : m_bindings) {
        descriptorSetLayoutBindings.push_back(binding);
    }

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
    descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(descriptorSetLayoutBindings.size());
    descriptorSetLayoutInfo.pBindings = descriptorSetLayoutBindings.data();

    if (vkCreateDescriptorSetLayout(m_device.device(), &descriptorSetLayoutInfo, nullptr, &m_descriptorSetLayout) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

GPUDescriptorSetLayout::~GPUDescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(m_device.device(), m_descriptorSetLayout, nullptr);
}

GPUDescriptorPool::Builder& GPUDescriptorPool::Builder::addPoolSize(VkDescriptorType t_descriptorType,
                                                                    uint32_t t_count) {
    m_poolSizes.push_back({t_descriptorType, t_count});
    return *this;
}

GPUDescriptorPool::Builder& GPUDescriptorPool::Builder::setPoolFlags(VkDescriptorPoolCreateFlags t_flags) {
    m_poolFlags = t_flags;
    return *this;
}
GPUDescriptorPool::Builder& GPUDescriptorPool::Builder::setMaxSets(uint32_t t_count) {
    m_maxSets = t_count;
    return *this;
}

std::unique_ptr<GPUDescriptorPool> GPUDescriptorPool::Builder::build() const {
    return std::make_unique<GPUDescriptorPool>(m_device, m_maxSets, m_poolFlags, m_poolSizes);
}

GPUDescriptorPool::GPUDescriptorPool(GPUDevice& t_device, uint32_t t_maxSets, VkDescriptorPoolCreateFlags t_poolFlags,
                                     const std::vector<VkDescriptorPoolSize>& t_poolSizes)
    : m_device{t_device} {
    VkDescriptorPoolCreateInfo descriptorPoolInfo{};
    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(t_poolSizes.size());
    descriptorPoolInfo.pPoolSizes = t_poolSizes.data();
    descriptorPoolInfo.maxSets = t_maxSets;
    descriptorPoolInfo.flags = t_poolFlags;

    if (vkCreateDescriptorPool(m_device.device(), &descriptorPoolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

GPUDescriptorPool::~GPUDescriptorPool() {
    vkDestroyDescriptorPool(m_device.device(), m_descriptorPool, nullptr);
}

} // namespace euclide