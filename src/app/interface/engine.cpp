#include "engine.h"

#include "gpu/gpu_manager.h"

namespace euclide {

Engine::Engine(const uint32_t t_width, const uint32_t t_height, const char* t_windowName)
    : m_scene(std::make_shared<euclide::Scene>()), m_window(t_width, t_height, t_windowName),
      m_interface(m_window.getWindow(), m_scene) {
    GPUManager::getInstance(); // Init Vulkan

    // Set GLFW user pointer to allow static callback access to this instance
    glfwSetWindowUserPointer(m_window.getWindow(), this);

    // Set window resize callback
    glfwSetWindowSizeCallback(m_window.getWindow(), Engine::resizeCallback);
}

void Engine::mainLoop() {
    while (!m_window.shouldClose()) {
        // Poll input events
        glfwPollEvents();

        // Render current frame
        draw();
    }
}

void Engine::draw() {
    // Draw the UI and scene
    m_interface.draw();

    // Swap buffers to display rendered frame
    m_window.swapBuffers();
}

void Engine::resizeCallback(GLFWwindow* window, int width, int height) {
    // Retrieve engine instance from GLFW user pointer
    auto* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (engine) {
        // Redraw frame on window resize
        engine->draw();
    }
}

} // namespace euclide
