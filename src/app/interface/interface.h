#pragma once

#include "panels/panel.h"
#include "nodes/scene.h"

#include <vector>
#include <memory>

namespace butter {

class Interface {

public:
    Interface(GLFWwindow* t_window, const std::shared_ptr<Scene>& t_scene);
    ~Interface();

    Interface(const Interface &) = delete;
    Interface &operator=(const Interface &) = delete;

    Interface(Interface &&) noexcept = default;
    Interface &operator=(Interface &&) noexcept = default;

    void addPanel(std::shared_ptr<Panel> t_panel);
    void addNodeGraph();
    void addViewport();

    void draw() const;

    std::shared_ptr<Scene> scene() const { return m_scene.lock(); }

private:
    void createDockSpace() const;
    void beginFrame() const;
    void renderFrame() const;
    void clearFrame() const;

    ImFont* m_font;
    

    std::weak_ptr<Scene> m_scene;
    std::vector<std::shared_ptr<Panel>> m_panels;

    static constexpr struct { float r, g, b, a; } s_bgColor = {0.3f, 0.3f, 0.3f, 1.0f};
};

}