#include "parameters.h"

namespace euclide {

Parameters::Parameters(const std::weak_ptr<NodeGraph> t_nodeGraph) : m_nodeGraph(t_nodeGraph) {
    if (auto nodeGraph = m_nodeGraph.lock()) {
        nodeGraph->onNodeSelected.subscribe([this](std::weak_ptr<Node> t_node) { m_node = t_node; });
    }
}

} // namespace euclide