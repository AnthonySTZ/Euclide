#include "viewport.h"

namespace butter {
    
Viewport::Viewport(const std::shared_ptr<Scene> &t_scene)
    : m_scene(t_scene), m_renderer(std::make_unique<Renderer>()), m_camera(std::make_shared<Camera>())
{
    if (auto scene = m_scene.lock()) {
        scene->onMeshUpdate.subscribe(
            [this](std::shared_ptr<Mesh> t_mesh) {
                m_renderer->updateMesh(t_mesh);
            }
        );
    }

    m_renderer->setCamera(m_camera);
}

void Viewport::draw()
{
    beginTab("Viewport", m_padding);
    updateFps();

    m_windowPosition = ImGui::GetCursorPos();
    checkForResize();

    handleMouse();

    drawRender();
    drawInfos();
    
    endTab();
}

void Viewport::handleMouse() {

    if (m_isItemHovered) {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            m_isLeftClicked = true;
        }
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle)) {
            m_isMiddleClicked = true;
        }
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            m_isRightClicked = true;
        }
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        m_isLeftClicked = false;
    }
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle)) {
        m_isMiddleClicked = false;
    }
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
        m_isRightClicked = false;
    }

    moveCamera();
    
}

void Viewport::moveCamera() {

    if (m_isLeftClicked) {
        ImVec2 dragDelta = ImGui::GetIO().MouseDelta;
        ImGui::SetMouseCursor(ImGuiMouseSource_Pen);
        m_camera->orbit(-dragDelta.x, -dragDelta.y);
    }

    if (m_isMiddleClicked) {
        ImVec2 dragDelta = ImGui::GetIO().MouseDelta;
        ImGui::SetMouseCursor(ImGuiMouseSource_Pen);
        m_camera->pan(-dragDelta.x, dragDelta.y);
    }

    if (m_isRightClicked) {
        ImVec2 dragDelta = ImGui::GetIO().MouseDelta;
        ImGui::SetMouseCursor(ImGuiMouseSource_Pen);
        m_camera->dolly(-dragDelta.y);
    }
}

void Viewport::drawRender() {
    m_renderer->draw(m_viewportWidth, m_viewportHeight);
    
    ImTextureID textureID = m_renderer->getRenderTexture();
    ImGui::Image(textureID, ImVec2((float)m_viewportWidth, (float)m_viewportHeight));

    m_isItemHovered = ImGui::IsItemHovered();
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