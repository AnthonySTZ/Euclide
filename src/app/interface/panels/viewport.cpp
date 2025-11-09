#include "viewport.h"

namespace butter {
    
Viewport::Viewport(const std::shared_ptr<Scene> &t_scene)
    : m_scene(t_scene)
{
    if (auto scene = m_scene.lock()) {

    }
}

void Viewport::draw()
{
    updateFps();

    beginTab("Viewport", m_padding);

    drawInfos();

    endTab();
}

void Viewport::drawInfos() const
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    ImVec2 top_left = ImGui::GetCursorPos();

    drawList->AddText(top_left, IM_COL32(255, 255, 255, 255), m_fpsText.c_str());
}

void Viewport::updateFps() {
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> delta = currentTime - m_lastTime;
    float elapsedTime = delta.count();
    m_frameCount++;

    if (elapsedTime >= m_refreshFpsEvery) {
        int fps = static_cast<uint32_t>(m_frameCount / elapsedTime);
        m_fpsText = std::string("Fps: ") + std::to_string(fps);
        
        m_frameCount = 0;
        m_lastTime = currentTime;
    }
}

}