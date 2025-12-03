#include "gpu_pipeline.h"

#include "utils/file.h"

namespace euclide {

GPUPipeline::GPUPipeline(GPUDevice& t_device, const std::string& t_shaderFile,
                         const std::vector<VkDescriptorSetLayoutBinding>& t_descriptorSetLayoutBindings)
    : m_device(t_device) {
    createShaderModule(t_shaderFile, &m_shaderModule);
    createDescriptorSetLayout(t_descriptorSetLayoutBindings);
    createPipeline();
}

GPUPipeline::~GPUPipeline() {
    vkDestroyDescriptorSetLayout(m_device.device(), m_descriptorSetLayout, nullptr);
    vkDestroyShaderModule(m_device.device(), m_shaderModule, nullptr);
    vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);
    vkDestroyPipelineCache(m_device.device(), m_pipelineCache, nullptr);
    vkDestroyPipeline(m_device.device(), m_pipeline, nullptr);
}

void GPUPipeline::createShaderModule(const std::string& t_shaderFile, VkShaderModule* t_shaderModule) {
    std::vector<char> shaderCode = readFile(t_shaderFile);

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

    if (vkCreateShaderModule(m_device.device(), &createInfo, nullptr, t_shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failde to create shader module");
    }

    std::cout << shaderCode.size() << '\n';
}

void GPUPipeline::createDescriptorSetLayout(
    const std::vector<VkDescriptorSetLayoutBinding>& t_descriptorSetLayoutBindings) {
    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
    descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(t_descriptorSetLayoutBindings.size());
    descriptorSetLayoutInfo.pBindings = t_descriptorSetLayoutBindings.data();

    if (vkCreateDescriptorSetLayout(m_device.device(), &descriptorSetLayoutInfo, nullptr, &m_descriptorSetLayout) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void GPUPipeline::createPipeline() {
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = 1;
    pipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(m_device.device(), &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout) !=
        VK_SUCCESS) {
        throw std::runtime_error("Failed to create PipelineLayout!");
    }

    VkPipelineCacheCreateInfo pipelineCacheCreateInfo{};
    pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

    if (vkCreatePipelineCache(m_device.device(), &pipelineCacheCreateInfo, nullptr, &m_pipelineCache) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Pipeline Cache!");
    }

    VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo{};
    pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    pipelineShaderStageCreateInfo.module = m_shaderModule;
    pipelineShaderStageCreateInfo.pName = "Main";
    pipelineShaderStageCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;

    VkComputePipelineCreateInfo computePipelineCreateInfo{};
    computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    computePipelineCreateInfo.layout = m_pipelineLayout;
    computePipelineCreateInfo.stage = pipelineShaderStageCreateInfo;

    if (vkCreateComputePipelines(m_device.device(), m_pipelineCache, 1, &computePipelineCreateInfo, nullptr,
                                 &m_pipeline) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Pipeline!");
    }
}

} // namespace euclide