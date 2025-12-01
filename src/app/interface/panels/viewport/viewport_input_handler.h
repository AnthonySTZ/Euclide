#pragma once

#include "viewport.h"

namespace butter {

class ViewportInputHandler {
  public:
    ViewportInputHandler(const std::weak_ptr<Viewport> t_viewport) : m_viewport(t_viewport) {}
    ~ViewportInputHandler() = default;

    void pollEvents();

  private:
    std::weak_ptr<Viewport> m_viewport;
};

} // namespace butter