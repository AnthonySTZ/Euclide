#pragma once

#include "node_item.h"

namespace euclide {

/// @brief Used to render a nodeItem
class NodeItemRenderer {
  public:
    NodeItemRenderer(const std::weak_ptr<NodeItemModel> t_model);

  private:
    std::weak_ptr<NodeItemModel> m_nodeModel;
};

} // namespace euclide