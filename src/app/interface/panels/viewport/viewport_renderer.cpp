#include "viewport_renderer.h"

namespace butter {

ViewportRenderer::ViewportRenderer(const std::weak_ptr<Viewport> t_viewport)
    : m_viewport(t_viewport), m_renderer(std::make_unique<Renderer>()) {
    if (auto viewport = m_viewport.lock()) {
        m_renderer->setCamera(viewport->camera());
    }
}

void ViewportRenderer::render() {
    checkForResize();
    draw();
}

void ViewportRenderer::checkForResize() {
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();

    if (m_viewportWidth != viewportSize.x || m_viewportHeight != viewportSize.y) {
        m_viewportWidth = viewportSize.x;
        m_viewportHeight = viewportSize.y;
        m_renderer->resizeFrameBuffer(m_viewportWidth, m_viewportHeight);
    }
}

void ViewportRenderer::draw() {
    m_renderer->beginFrame(m_viewportWidth, m_viewportHeight);
    m_renderer->clearFrame();

    if (auto viewport = m_viewport.lock()) {
        m_renderer->draw(viewport->gridModel());
        m_renderer->draw(viewport->renderModel());
    }

    m_renderer->endFrame(m_viewportWidth, m_viewportHeight);

    ImTextureID textureID = m_renderer->getRenderTexture();
    ImGui::Image(textureID, ImVec2((float)m_viewportWidth, (float)m_viewportHeight));
}

} // namespace butter
