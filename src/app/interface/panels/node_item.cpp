#include "node_item.h"

namespace butter {
    
NodeItem::NodeItem(const std::shared_ptr<Node> &t_node, const ImVec2 t_position)
    : m_node(t_node), m_position(t_position)
{}

void NodeItem::draw()
{
    if (auto node = m_node.lock()) {

        std::cout << node->name() << "\n"; 
        
    }
}

}