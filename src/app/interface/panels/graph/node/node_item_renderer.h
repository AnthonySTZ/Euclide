#pragma once

#include "node_item.h"

namespace euclide {

/// @brief Used to render a nodeItem
class NodeItemRenderer {
  public:
    NodeItemRenderer(const std::weak_ptr<NodeItemModel> t_model);

    void render();

  private:
    void drawRect(const std::shared_ptr<NodeItemModel>& t_nodeModel) const;

  private:
    std::weak_ptr<NodeItemModel> m_nodeModel;

    std::vector<ImVec2> m_inputIOPositions;  ///< Cached input slot positions.
    std::vector<ImVec2> m_outputIOPositions; ///< Cached output slot positions.
};

} // namespace euclide