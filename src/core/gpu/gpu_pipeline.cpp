#include "gpu_pipeline.h"

#include "utils/file.h"

namespace euclide {

GPUPipeline::GPUPipeline(GPUDevice& t_device, const std::string& t_shaderFile) : m_device(t_device) {
    createShaderModule(t_shaderFile, &m_shaderModule);
}

GPUPipeline::~GPUPipeline() {
    vkDestroyShaderModule(m_device.device(), m_shaderModule, nullptr);
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

} // namespace euclide