#pragma once

#include "window.h"
#include "interface.h"


namespace butter {

class Engine {

public:
    Engine(const uint32_t t_width, const uint32_t t_height, const char *t_windowName);
    ~Engine() = default;

    void mainLoop();

private:
    void draw();
    static void resizeCallback(GLFWwindow *window, int width, int height);

    Window m_window;
    Interface m_interface;
};


}