#pragma once

#include "nodes/node.h"

#include "interface/utils/imgui_utils.h"
#include <memory>

namespace butter {

class NodeItem {

public:
    NodeItem(const std::shared_ptr<Node>& t_node, const ImVec2 t_position);

    void draw();
private:
    std::weak_ptr<Node> m_node;
    ImVec2 m_position;

};


}