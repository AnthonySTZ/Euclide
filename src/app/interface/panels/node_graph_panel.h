#pragma once

#include "interface/panels/panel.h"

#include "graph/node_graph_renderer.h"
#include "graph/node_graph_input_handler.h"
#include "graph/node_graph_menu.h"

namespace butter {

class NodeGraphPanel : public Panel {
  public:
    NodeGraphPanel(const std::shared_ptr<Scene> t_scene);
    ~NodeGraphPanel() override = default;

    void draw() override;

    [[nodiscard]] std::weak_ptr<NodeGraph> graph() { return m_graph; }

  private:
    std::shared_ptr<NodeGraph> m_graph;
    NodeGraphRenderer m_graphRenderer;
    NodeGraphInputHandler m_graphInputHandler;
    NodeGraphMenu m_graphMenu;
};

} // namespace butter