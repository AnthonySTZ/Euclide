#include "cutting_line.h"

namespace butter {
    
void CuttingLine::draw() const noexcept
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddLine(m_start, m_end, s_color, s_thickness);
}

}