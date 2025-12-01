#pragma once

#include "viewport.h"

#include "interface/utils/imgui_utils.h"

namespace butter {

class ViewportInputHandler {
  public:
    ViewportInputHandler(const std::weak_ptr<Viewport> t_viewport) : m_viewport(t_viewport) {}
    ~ViewportInputHandler() = default;

    void pollEvents();

  private:
    void handleMouseInputs();

  private:
    std::weak_ptr<Viewport> m_viewport;

    bool m_isWindowHovered = false;

    bool m_isLeftMouseButtonClicked = false;
    bool m_isMiddleMouseButtonClicked = false;
    bool m_isRightMouseButtonClicked = false;
};

} // namespace butter