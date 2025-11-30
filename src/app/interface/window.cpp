#include "window.h"

#include <iostream>

namespace butter {

Window::Window(uint32_t t_width, uint32_t t_height, const char* t_name) {
    if (t_width <= 0 || t_height <= 0) {
        throw std::runtime_error("Window size invalid!");
    }

    if (!glfwInit()) {
        throw std::runtime_error("Failed initialized GLFW!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(t_width, t_height, t_name, NULL, NULL);
    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("Failed creating Window!");
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(0); // Disable Vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD!");
    }

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << err << "\n";
    }
}

Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

Window::Window(Window&& t_other) noexcept : m_window(t_other.m_window) {
    t_other.m_window = nullptr;
}

Window& Window::operator=(Window&& t_other) noexcept {
    if (this != &t_other) {
        if (m_window != nullptr) {
            glfwDestroyWindow(m_window);
        }
        m_window = t_other.m_window;
        t_other.m_window = nullptr;
    }
    return *this;
}

GLFWwindow* Window::getWindow() const noexcept {
    return m_window;
}

bool Window::shouldClose() const noexcept {
    return glfwWindowShouldClose(m_window);
}

void Window::swapBuffers() {
    glfwSwapBuffers(m_window);
}

} // namespace butter