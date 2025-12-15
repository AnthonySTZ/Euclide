#pragma once

#include "panels/panel.h"
#include "nodes/scene.h"

#include "panels/viewport_panel.h"
#include "panels/node_graph_panel.h"
#include "panels/parameters_panel.h"
#include "panels/spreadsheet_panel.h"

#include <vector>
#include <memory>

namespace euclide {

/// @brief Manages the GUI interface of the engine using ImGui.
class Interface {
  public:
    /// @brief Constructs the Interface and initializes ImGui.
    /// @param t_window Pointer to the GLFW window.
    /// @param t_scene Shared pointer to the Scene.
    Interface(GLFWwindow* t_window, const std::shared_ptr<Scene>& t_scene);

    /// @brief Destructor that shuts down ImGui and cleans up resources.
    ~Interface();

    Interface(const Interface&) = delete;            ///< Delete copy constructor
    Interface& operator=(const Interface&) = delete; ///< Delete copy assignment

    Interface(Interface&&) noexcept = default;            ///< Move constructor
    Interface& operator=(Interface&&) noexcept = default; ///< Move assignment

    /// @brief Adds a custom panel to the interface.
    /// @param t_panel Shared pointer to a Panel object.
    void addPanel(std::shared_ptr<Panel> t_panel);

    /// @brief Adds a NodeGraph panel connected to the Scene.
    void addNodeGraph();

    /// @brief Adds a Viewport panel connected to the Scene.
    void addViewport();

    /// @brief Adds a Parameters panel connected to the NodeGraph.
    void addParameters();

    /// @brief Adds a Spreadsheet panel connected to the scene.
    void addSpreadsheet();

    /// @brief Draws all panels and renders the GUI.
    void draw() const;

    /// @brief Returns a shared pointer to the Scene.
    [[nodiscard]] std::shared_ptr<Scene> scene() const { return m_scene.lock(); }

  private:
    /// @brief Creates the ImGui docking space.
    void createDockSpace() const;

    /// @brief Begins a new ImGui frame.
    void beginFrame() const;

    /// @brief Renders the ImGui frame.
    void renderFrame() const;

    /// @brief Clears the OpenGL frame buffer.
    void clearFrame() const;

  private:
    ImFont* m_font; ///< Pointer to the loaded font

    std::weak_ptr<Scene> m_scene;                 ///< Weak reference to the Scene
    std::vector<std::shared_ptr<Panel>> m_panels; ///< Container of all GUI panels

    std::shared_ptr<ViewportPanel> m_viewport;       ///< Viewport panel
    std::shared_ptr<NodeGraphPanel> m_nodeGraph;     ///< NodeGraph panel
    std::shared_ptr<ParametersPanel> m_parameters;   ///< Parameters panel
    std::shared_ptr<SpreadsheetPanel> m_spreadsheet; ///< Spreadsheet panel

    static constexpr struct { float r, g, b, a; } BG_COLOR = {0.3f, 0.3f, 0.3f, 1.0f}; ///< Background color
};

} // namespace euclide