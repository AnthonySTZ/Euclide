#pragma once

#include "parameters.h"

namespace euclide {

class ParametersRenderer {
  public:
    ParametersRenderer(const std::weak_ptr<Parameters> t_parameters);
    void draw();

  private:
    void drawParameters();
    void drawTitleName(const std::string& t_title);
    void pushStyle();
    void popStyle();

  private:
    std::weak_ptr<Parameters> m_parameters;

    static constexpr float TITLE_SPACING = 20.0f; ///< Vertical spacing after node title.
};

} // namespace euclide