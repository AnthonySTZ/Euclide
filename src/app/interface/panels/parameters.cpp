#include "parameters.h"

namespace butter {

Parameters::Parameters(const std::shared_ptr<NodeGraph> &t_nodeGraph)
    : m_nodeGraph(t_nodeGraph)
{}

void Parameters::draw()
{
    beginTab("Parameters", m_padding);
    endTab();
}

}

