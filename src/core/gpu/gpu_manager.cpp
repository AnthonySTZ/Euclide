#include "gpu_manager.h"

namespace euclide {

GPUManager& GPUManager::getInstance() {
    static GPUManager instance;
    return instance;
}

GPUManager::GPUManager() {
    initVulkan();
    m_device = std::make_unique<GPUDevice>(m_vkInstance);
}

GPUManager::~GPUManager() {
    m_device->destroy();
    if (m_vkInstance != VK_NULL_HANDLE) {
        vkDestroyInstance(m_vkInstance, nullptr);
    }
}

void GPUManager::initVulkan() {
    VkApplicationInfo appInfo{}; // This struct is optional and store some informations about the app
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Euclide";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Euclide";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    VkInstanceCreateInfo createInfo{}; // Not optional, store info to create the Vulkan Instance
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    vkCreateInstance(&createInfo, nullptr, &m_vkInstance);
}

} // namespace euclide