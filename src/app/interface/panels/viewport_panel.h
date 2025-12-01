#pragma once

#include "interface/panels/panel.h"

#include "interface/panels/viewport/viewport.h"
#include "interface/panels/viewport/viewport_renderer.h"
#include "interface/panels/viewport/viewport_input_handler.h"

namespace butter {

class ViewportPanel : public Panel {
  public:
    ViewportPanel(const std::weak_ptr<Scene> t_scene)
        : m_viewport(std::make_shared<Viewport>(t_scene)), m_viewportRenderer(m_viewport),
          m_viewportInputHandler(m_viewport) {}
    ~ViewportPanel() = default;

    void draw() override;

  private:
    std::shared_ptr<Viewport> m_viewport;
    ViewportRenderer m_viewportRenderer;
    ViewportInputHandler m_viewportInputHandler;
};

} // namespace butter
