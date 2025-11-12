#pragma once

#include "interface/utils/imgui_utils.h"

namespace butter {

class CuttingLine {

public:
    CuttingLine(const ImVec2& t_start, const ImVec2& t_end) : m_start{t_start}, m_end{t_end} {}

    void draw() const noexcept;

private:
    ImVec2 m_start;
    ImVec2 m_end;
    const static constexpr ImU32 s_color = IM_COL32(255, 50, 50, 255);
    const static constexpr float s_thickness = 1.5f;

};

}