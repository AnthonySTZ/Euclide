#pragma once

#include "panel.h"

#include "node_graph.h"

namespace butter {

class Parameters : public Panel {

public:
    Parameters(const std::shared_ptr<NodeGraph>& t_nodeGraph);
    ~Parameters() = default;

    void draw() override;

private:
    
    void drawParameters();
    std::weak_ptr<NodeGraph> m_nodeGraph;
    std::weak_ptr<Node> m_node;
};

}