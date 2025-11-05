#pragma once

#include "window.h"
#include "interface.h"
#include "nodes/scene.h"

namespace butter {

class Engine {

public:
    Engine(const uint32_t t_width, const uint32_t t_height, const char *t_windowName);
    ~Engine() = default;

    void mainLoop();

    Interface& interface() { return m_interface; }
    std::shared_ptr<Scene> scene() const { return m_scene; }

private:
    void draw();
    static void resizeCallback(GLFWwindow *window, int width, int height);

    Window m_window;
    std::shared_ptr<Scene> m_scene;
    Interface m_interface;
};


}