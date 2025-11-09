#pragma once 

#include "nodes/scene.h"
#include "panel.h"

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
    void drawInfos() const;

    void updateFps();

    std::weak_ptr<Scene> m_scene;

    // Fps requirements
    float m_refreshFpsEvery = .5f;
    std::chrono::steady_clock::time_point m_lastTime;
    uint32_t m_frameCount = 0;
    std::string m_fpsText;

};


}