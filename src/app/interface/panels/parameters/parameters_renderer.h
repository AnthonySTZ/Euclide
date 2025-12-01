#pragma once

#include "parameters.h"

namespace euclide {

class ParametersRenderer {
  public:
    ParametersRenderer(const std::weak_ptr<Parameters> t_parameters);

    /// @brief Draw Parameters with correct style
    void draw();

  private:
    /// @brief Draw parameters for the current selected node
    void drawParameters();

    /// @brief Draw the name of the selected node
    /// @param t_title
    void drawTitleName(const std::string& t_title);

    /// @brief Push ImGui Parameters style
    void pushStyle();

    /// @brief Pop ImGui Parameters style
    void popStyle();

  private:
    std::weak_ptr<Parameters> m_parameters;

    static constexpr float TITLE_SPACING = 20.0f; ///< Vertical spacing after node title.
};

} // namespace euclide