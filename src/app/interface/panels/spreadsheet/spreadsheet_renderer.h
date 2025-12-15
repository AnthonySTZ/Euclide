#pragma once

#include "spreadsheet.h"

namespace euclide {

class SpreadsheetRenderer {
  public:
    SpreadsheetRenderer(const std::weak_ptr<Spreadsheet> t_spreasheet);

    void render();

  private:
    std::weak_ptr<Spreadsheet> m_spreadsheet;
};

} // namespace euclide