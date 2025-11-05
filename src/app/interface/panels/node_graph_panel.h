#pragma once

#include "panel.h"
#include "nodes/scene.h"
#include "node_item.h"

namespace butter {

class NodeGraph : public Panel {

public:
    NodeGraph(const std::shared_ptr<Scene>& t_scene);
    ~NodeGraph() = default;

    void draw() override;
    
private:
    void handleInputs();
    void handleCreateNode();
    void handleNodeDragging();
    void drawNodes();

    std::weak_ptr<Scene> m_scene;

    std::unordered_map<uint32_t, std::shared_ptr<NodeItem>> m_nodeItems;
    
    bool m_isDrag = false;
    std::shared_ptr<NodeItem> m_nodeHovered = nullptr;
};

}