#include "viewport_renderer.h"

#include "interface/utils/string_utils.h"

namespace euclide {

ViewportRenderer::ViewportRenderer(const std::weak_ptr<Viewport> t_viewport)
    : m_viewport(t_viewport), m_renderer(std::make_unique<Renderer>()) {
    if (auto viewport = m_viewport.lock()) {
        m_renderer->setCamera(viewport->camera());
    }
}

void ViewportRenderer::render() {
    checkForResize();

    m_windowPosition = ImGui::GetWindowPos();
    draw();
    drawInfos();
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

void ViewportRenderer::drawInfos() {
    auto viewport = m_viewport.lock();
    if (!viewport)
        return;

    m_infoOffset = INFO_POS;

    ImGuiIO& io = ImGui::GetIO();
    std::string fpsText = "Fps: " + thousandSeparator(static_cast<int>(io.Framerate));
    addInfos(fpsText);
    addInfos("Points: " + thousandSeparator(viewport->renderModel().numOfPoints()));
    addInfos("Primitives: " + thousandSeparator(viewport->renderModel().numOfPrims()));
    addInfos("P: Toogle Primitives");
    addInfos("W: Toogle Wireframe");
    addInfos("V: Toogle Points");
    addInfos("G: Toogle Grid");
}

void ViewportRenderer::addInfos(const std::string& t_infos) {
    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    if (!drawList)
        return;

    drawList->AddText(m_windowPosition + m_infoOffset, INFO_COLOR, t_infos.c_str());
    m_infoOffset += INFO_PADDING;
}

} // namespace euclide
