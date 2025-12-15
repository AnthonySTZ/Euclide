#pragma once

#include "spreadsheet.h"
#include "interface/utils/imgui_utils.h"

namespace euclide {

class SpreadsheetRenderer {
  public:
    SpreadsheetRenderer(const std::weak_ptr<Spreadsheet> t_spreasheet);

    void render();

  private:
    void renderTable();

    void drawTable(const AttributeSet& t_attributes);

  private:
    std::weak_ptr<Spreadsheet> m_spreadsheet;

    float m_tableScroll = 0.0f;

    static constexpr ImGuiWindowFlags CHILD_FLAGS = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
    static constexpr ImGuiWindowFlags TABLE_FLAGS = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
};

} // namespace euclide