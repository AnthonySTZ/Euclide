#include "viewport_panel.h"

void butter::ViewportPanel::draw() {
    beginTab("Viewport", m_padding);

    m_viewportInputHandler.pollEvents();
    m_viewportRenderer.render();

    endTab();
}