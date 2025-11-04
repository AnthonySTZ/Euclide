#pragma once

#include "utils/imgui_utils.h"

namespace butter {

class Interface {

public:
    Interface(GLFWwindow* t_window);
    ~Interface();

private:
    ImFont* m_font;

};

}