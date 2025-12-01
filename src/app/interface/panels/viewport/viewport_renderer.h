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
    void drawInfos();
    void addInfos(const std::string& t_infos);

  private:
    std::weak_ptr<Viewport> m_viewport;
    std::unique_ptr<Renderer> m_renderer;
    uint32_t m_viewportWidth, m_viewportHeight; ///< Current viewport size.

    ImVec2 m_windowPosition{0, 0};
    ImVec2 m_infoOffset{10, 10};
    static constexpr ImVec2 INFO_POS = ImVec2(10, 10);
    static constexpr ImU32 INFO_COLOR = IM_COL32(255, 255, 255, 255);
    static constexpr ImVec2 INFO_PADDING = ImVec2(0, 25);
};

} // namespace butter