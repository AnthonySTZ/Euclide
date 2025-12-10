#pragma once

#include "node_item_model.h"
#include "node_item_renderer.h"
#include "node_item_interaction.h"

namespace euclide {

class NodeItem {
  public:
    NodeItem(const std::weak_ptr<Node> t_node, const ImVec2 t_position);
    ~NodeItem() = default;

    void draw();

    [[nodiscard]] inline std::shared_ptr<NodeItemModel> model() noexcept { return m_model; }

  private:
    std::shared_ptr<NodeItemModel> m_model;
    NodeItemRenderer m_renderer;
};

} // namespace euclide