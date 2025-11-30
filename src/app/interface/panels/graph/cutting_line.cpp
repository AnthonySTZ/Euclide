#include "cutting_line.h"

namespace butter {

void CuttingLine::draw() const noexcept
{
    if (ImDrawList* drawList = ImGui::GetWindowDrawList()) {
        drawList->AddLine(m_start, m_end, COLOR, THICKNESS);
    }
}

} // namespace butter
