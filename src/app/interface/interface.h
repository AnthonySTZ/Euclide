#pragma once

#include "panels/panel.h"


#include <vector>
#include <memory>

namespace butter {

class Interface {

public:
    Interface(GLFWwindow* t_window);
    ~Interface();

    Interface(const Interface &) = delete;
    Interface &operator=(const Interface &) = delete;

    Interface(Interface &&) noexcept = default;
    Interface &operator=(Interface &&) noexcept = default;

    void addPanel(std::shared_ptr<Panel> t_panel);
    void draw() const;

private:
    void createDockSpace() const;
    void beginFrame() const;
    void renderFrame() const;
    void clearFrame() const;

    ImFont* m_font;
    
    std::vector<std::shared_ptr<Panel>> m_panels;

    static constexpr struct { float r, g, b, a; } s_bgColor = {0.3f, 0.3f, 0.3f, 1.0f};
};

}