#include "spreadsheet_panel.h"

namespace euclide {

SpreadsheetPanel::SpreadsheetPanel(std::weak_ptr<Scene> t_scene)
    : m_spreadsheet(std::make_shared<Spreadsheet>(t_scene)), m_spreadsheetRenderer(m_spreadsheet) {
}

void SpreadsheetPanel::draw() {
    beginTab("Spreasheet", m_padding);

    m_spreadsheetRenderer.render();

    endTab();
}

} // namespace euclide
