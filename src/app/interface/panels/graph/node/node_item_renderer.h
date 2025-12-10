#pragma once

#include "node_item.h"

namespace euclide {

/// @brief Used to render a nodeItem
class NodeItemRenderer {
  public:
    static void draw(const NodeItem& t_nodeItem);

  private:
    static void drawRect(const NodeItem& t_nodeItem);
};

} // namespace euclide