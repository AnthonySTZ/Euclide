#pragma once

#include "interface/panels/graph/node_graph.h"

namespace euclide {

class Parameters {
  public:
    Parameters(const std::weak_ptr<NodeGraph> t_nodeGraph);

    [[nodiscard]] std::weak_ptr<Node> node() const noexcept { return m_node; }

  private:
    std::weak_ptr<NodeGraph> m_nodeGraph = std::weak_ptr<NodeGraph>(); ///< Observed NodeGraph for node selection.
    std::weak_ptr<Node> m_node = std::weak_ptr<Node>();
};

} // namespace euclide