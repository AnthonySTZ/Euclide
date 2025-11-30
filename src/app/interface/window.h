#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace butter {

/// @brief Wrapper class for managing a GLFW window and OpenGL context.
class Window {
  public:
    /// @brief Constructs a Window with the specified size and title.
    /// @param t_width Width of the window in pixels.
    /// @param t_height Height of the window in pixels.
    /// @param t_name Title of the window.
    /// @throws std::runtime_error if GLFW or GLAD fails to initialize, or window creation fails.
    Window(uint32_t t_width, uint32_t t_height, const char* t_name);

    /// @brief Destroys the window and terminates GLFW.
    ~Window();

    // Deleted copy operations
    Window(const Window&) = delete;            ///< Deleted copy constructor
    Window& operator=(const Window&) = delete; ///< Deleted copy assignment

    // Movable
    Window(Window&& t_other) noexcept;            ///< Move constructor
    Window& operator=(Window&& t_other) noexcept; ///< Move assignment

    /// @brief Returns the underlying GLFW window pointer.
    /// @return Pointer to GLFWwindow.
    [[nodiscard]] GLFWwindow* getWindow() const noexcept;

    /// @brief Checks whether the window should close.
    /// @return True if the window should close, false otherwise.
    [[nodiscard]] bool shouldClose() const noexcept;

    /// @brief Swaps the front and back buffers.
    void swapBuffers();

  private:
    GLFWwindow* m_window; ///< Pointer to the GLFW window
};

} // namespace butter
