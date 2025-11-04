#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace butter {

class Window {

public:
    Window(uint32_t t_width, uint32_t t_height, const char *t_name);
    ~Window();

    // No copyable
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    // Movable
    Window(Window &&t_other) noexcept;
    Window &operator=(Window &&t_other) noexcept;

    GLFWwindow *getWindow() const noexcept;
    bool shouldClose() const noexcept;
    void swapBuffers();

private:
    GLFWwindow *m_window;
};

    
}