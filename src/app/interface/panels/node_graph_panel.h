#pragma once

#include "interface/panels/panel.h"

#include "graph/node_graph_renderer.h"

namespace butter {

class NodeGraphPanel : public Panel {
  public:
    NodeGraphPanel(const std::shared_ptr<Scene> t_scene)
        : m_graph(std::make_shared<NodeGraph>(std::move(t_scene))), m_graphRenderer(m_graph){};
    ~NodeGraphPanel() override = default;

    void draw() override;

    [[nodiscard]] std::weak_ptr<NodeGraph> graph() { return m_graph; }

  private:
    std::shared_ptr<NodeGraph> m_graph;
    NodeGraphRenderer m_graphRenderer;
};

} // namespace butter