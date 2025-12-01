#pragma once

#include "window.h"
#include "interface.h"
#include "nodes/scene.h"

#include <memory>

namespace euclide {

/// @brief Core application engine handling the main loop, window, and interface.
class Engine {
  public:
    /// @brief Construct the engine with window dimensions and title.
    /// @param t_width Window width in pixels.
    /// @param t_height Window height in pixels.
    /// @param t_windowName Title of the window.
    Engine(const uint32_t t_width, const uint32_t t_height, const char* t_windowName);

    ~Engine() = default;

    /// @brief Run the main loop until the window is closed.
    void mainLoop();

    /// @brief Access the UI interface.
    /// @return Reference to the Interface instance.
    Interface& interface() { return m_interface; }

    /// @brief Get the shared scene.
    /// @return Shared pointer to the Scene.
    std::shared_ptr<Scene> scene() const { return m_scene; }

  private:
    /// @brief Draw the current frame (UI + scene rendering).
    void draw();

    /// @brief GLFW callback for window resizing.
    /// @param window Pointer to GLFW window.
    /// @param width New window width.
    /// @param height New window height.
    static void resizeCallback(GLFWwindow* window, int width, int height);

  private:
    Window m_window;                ///< Application window.
    std::shared_ptr<Scene> m_scene; ///< Scene graph managed by the engine.
    Interface m_interface;          ///< User interface.
};

} // namespace euclide
