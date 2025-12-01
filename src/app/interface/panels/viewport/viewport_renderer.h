#pragma once

#include "viewport.h"

namespace butter {

class ViewportRenderer {
  public:
    ViewportRenderer(const std::weak_ptr<Viewport> t_viewport) : m_viewport(t_viewport) {}
    ~ViewportRenderer() = default;

    void render();

  private:
    std::weak_ptr<Viewport> m_viewport;
};

} // namespace butter