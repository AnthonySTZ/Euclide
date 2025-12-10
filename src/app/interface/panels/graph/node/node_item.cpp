#include "node_item.h"

namespace euclide {

NodeItem::NodeItem(const std::weak_ptr<Node> t_node, const ImVec2 t_position)
    : m_model(std::make_shared<NodeItemModel>(t_node, t_position)), m_renderer(m_model) {
}

void NodeItem::draw() {
    m_renderer.render();
}

} // namespace euclide