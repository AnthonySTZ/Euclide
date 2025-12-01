#include "parameters_panel.h"

namespace euclide {

ParametersPanel::ParametersPanel(const std::weak_ptr<NodeGraph> t_nodeGraph)
    : m_parameters(std::make_shared<Parameters>(t_nodeGraph)), m_parametersRenderer(m_parameters) {
}

void ParametersPanel::draw() {
    beginTab("Parameters", m_padding);

    m_parametersRenderer.draw();

    endTab();
}

} // namespace euclide