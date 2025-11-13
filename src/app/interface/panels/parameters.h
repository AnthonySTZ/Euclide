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
    std::weak_ptr<NodeGraph> m_nodeGraph;
};

}