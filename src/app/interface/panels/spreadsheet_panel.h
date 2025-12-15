#pragma once

#include "interface/panels/spreadsheet/spreadsheet_renderer.h"
#include "panel.h"

namespace euclide {

class SpreadsheetPanel : public Panel {
  public:
    SpreadsheetPanel(std::weak_ptr<Scene> t_scene);
    ~SpreadsheetPanel() = default;

    void draw() override;

  private:
    ImVec2 m_padding{0, 0};

    std::shared_ptr<Spreadsheet> m_spreadsheet;
    SpreadsheetRenderer m_spreadsheetRenderer;
};

} // namespace euclide