#pragma once 

#include "nodes/scene.h"
#include "panel.h"
#include "rendering/renderer.h"

#include <memory>
#include <chrono>
#include <string>

namespace butter {

class Viewport : public Panel {

public:
    Viewport(const std::shared_ptr<Scene>& t_scene);
    ~Viewport() = default;

    void draw() override;

private:
    void handleMouse();
    void moveCamera();
    void drawRender();
    void checkForResize();
    void drawInfos() const;

    void updateFps();

    std::weak_ptr<Scene> m_scene;

    ImVec2 m_windowPosition;

    // Rendering requirements
    std::unique_ptr<Renderer> m_renderer;
    uint32_t m_viewportWidth, m_viewportHeight;

    // Fps requirements
    float m_refreshFpsEvery = .5f;
    std::chrono::steady_clock::time_point m_lastTime;
    uint32_t m_frameCount = 0;
    std::string m_fpsText;

    ImVec2 m_padding {0, 0};
    std::shared_ptr<Camera> m_camera;

    bool m_isItemHovered = false;

    bool m_isLeftClicked = false;
    bool m_isMiddleClicked = false;
    bool m_isRightClicked = false;

};


}