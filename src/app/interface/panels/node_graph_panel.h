#pragma once

#include "panel.h"
#include "nodes/scene.h"

namespace butter {

struct NodeItem{
    std::weak_ptr<Node> m_node;
};

class NodeGraph : public Panel {

public:
    NodeGraph(const std::shared_ptr<Scene>& t_scene);
    ~NodeGraph() = default;

    void draw() override;

private:
    std::weak_ptr<Scene> m_scene;

    std::unordered_map<uint32_t, std::shared_ptr<NodeItem>> m_nodeItems;

};

}