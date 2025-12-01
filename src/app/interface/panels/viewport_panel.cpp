#include "viewport_panel.h"

namespace butter {

ViewportPanel::ViewportPanel(const std::weak_ptr<Scene> t_scene)
    : m_viewport(std::make_shared<Viewport>(t_scene)), m_viewportRenderer(m_viewport),
      m_viewportInputHandler(m_viewport) {
}

void ViewportPanel::draw() {
    beginTab("Viewport", m_padding);

    m_viewportInputHandler.pollEvents();
    m_viewportRenderer.render();

    endTab();
}

} // namespace butter