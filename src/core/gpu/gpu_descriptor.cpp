#include "gpu_descriptor.h"

namespace euclide {

GPUDescriptorSetLayout::Builder& GPUDescriptorSetLayout::Builder::addBinding(uint32_t t_binding,
                                                                             VkDescriptorType t_descriptorType,
                                                                             VkShaderStageFlags t_stageFlags,
                                                                             uint32_t t_count) {
    assert(m_bindings.find(t_binding) != m_bindings.end() && "Binding already in use");
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = t_binding;
    layoutBinding.descriptorType = t_descriptorType;
    layoutBinding.descriptorCount = t_count;
    layoutBinding.stageFlags = t_stageFlags;
    m_bindings[t_binding] = layoutBinding;
    return *this;
}

} // namespace euclide