#pragma once 

#include "interface/utils/imgui_utils.h"

namespace butter {

class Panel {

public:
    Panel() = default;
    virtual ~Panel() = default;

    virtual void draw() = 0;

protected:
    void beginTab(const char *t_name, ImVec2 t_padding = ImVec2(0.0, 0.0),
                  ImU32 t_bgCol = IM_COL32(30, 30, 30, 255)) const;
    void endTab() const;

    static constexpr float s_tabTitleHeight = 25.0f;
    static constexpr ImU32 s_tabTitleBgColor = IM_COL32(50, 50, 50, 255);
    static constexpr ImU32 s_tabTitleTextColor = IM_COL32(230, 230, 230, 255);
    static constexpr ImVec2 s_tabTitleTextPadding = {10.0f, 4.5f};

    ImVec2 m_padding = {10.f, 10.f};

};


}