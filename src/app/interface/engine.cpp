#include "engine.h"

namespace butter {
    
Engine::Engine(const uint32_t t_width, const uint32_t t_height, const char *t_windowName)
    : m_window(t_width, t_height, t_windowName)
{
    glfwSetWindowUserPointer(m_window.getWindow(), this);
    glfwSetWindowSizeCallback(m_window.getWindow(), Engine::resizeCallback);
}

void Engine::mainLoop() {
    while (!m_window.shouldClose()) {
        glfwPollEvents();
        draw();
    }
}

void Engine::draw() {
    m_window.swapBuffers();
}

void Engine::resizeCallback(GLFWwindow *window, int width, int height) {
    auto *engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (engine) {
        engine->draw();
    }
}

}