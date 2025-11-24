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
    void handleKeys();
    void retargetCamera();
    void moveCamera();
    void drawRender();
    void checkForResize();
    void drawInfos() const;

    std::weak_ptr<Scene> m_scene;

    ImVec2 m_windowPosition;

    // Rendering requirements
    std::unique_ptr<Renderer> m_renderer;
    RenderModel m_renderModel;
    RenderModel m_gridModel;
    uint32_t m_viewportWidth, m_viewportHeight;

    ImVec2 m_padding {0, 0};
    std::shared_ptr<Camera> m_camera;

    bool m_isItemHovered = false;

    bool m_isLeftClicked = false;
    bool m_isMiddleClicked = false;
    bool m_isRightClicked = false;

};


}