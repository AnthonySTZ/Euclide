#pragma once

#include "viewport.h"

#include "rendering/renderer.h"

namespace butter {

class ViewportRenderer {
  public:
    ViewportRenderer(const std::weak_ptr<Viewport> t_viewport);
    ~ViewportRenderer() = default;

    void render();

  private:
    void checkForResize();
    void draw();

  private:
    std::weak_ptr<Viewport> m_viewport;
    std::unique_ptr<Renderer> m_renderer;
    uint32_t m_viewportWidth, m_viewportHeight; ///< Current viewport size.
};

} // namespace butter