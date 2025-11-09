#include "viewport.h"

namespace butter {
    
Viewport::Viewport(const std::shared_ptr<Scene> &t_scene)
    : m_scene(t_scene), m_renderer(std::make_unique<Renderer>())
{
    if (auto scene = m_scene.lock()) {

    }
}

void Viewport::draw()
{
    updateFps();

    beginTab("Viewport", m_padding);
    m_windowPosition = ImGui::GetCursorPos();
    checkForResize();
    drawRender();
    
    drawInfos();
    endTab();
}

void Viewport::drawRender() const {
    m_renderer->draw(m_viewportWidth, m_viewportHeight);
    
    ImTextureID textureID = m_renderer->getRenderTexture();
    ImGui::Image(textureID, ImVec2((float)m_viewportWidth, (float)m_viewportHeight));
}

void Viewport::checkForResize() {
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    
    if (m_viewportWidth != viewportSize.x || m_viewportHeight != viewportSize.y) {
        m_viewportWidth = viewportSize.x;
        m_viewportHeight = viewportSize.y;
        m_renderer->resizeFrameBuffer(m_viewportWidth, m_viewportHeight);
    }
}

void Viewport::drawInfos() const
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddText(m_windowPosition + ImVec2(10, 10), IM_COL32(255, 255, 255, 255), m_fpsText.c_str());
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