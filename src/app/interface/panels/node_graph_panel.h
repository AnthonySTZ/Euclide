#pragma once

#include "interface/panels/panel.h"

#include "node_graph_renderer.h"

namespace butter {

class NodeGraphPanel : public Panel {
  public:
    NodeGraphPanel(const std::shared_ptr<Scene> t_scene) : m_graph(t_scene), m_graphRenderer(m_graph){};
    ~NodeGraphPanel() = default;

    void draw() override;

  private:
    NodeGraph m_graph;
    NodeGraphRenderer m_graphRenderer;
};

} // namespace butter