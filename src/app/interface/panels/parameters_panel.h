#pragma once

#include "interface/panels/panel.h"

#include "interface/panels/parameters/parameters.h"
#include "interface/panels/parameters/parameters_renderer.h"

namespace euclide {

class ParametersPanel : public Panel {
  public:
    ParametersPanel(const std::weak_ptr<NodeGraph> t_nodeGraph);
    ~ParametersPanel() = default;

    void draw() override;

  private:
    std::shared_ptr<Parameters> m_parameters;
    ParametersRenderer m_parametersRenderer;
};

} // namespace euclide