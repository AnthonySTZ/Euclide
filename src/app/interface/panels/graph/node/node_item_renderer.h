#pragma once

#include "node_item_model.h"

namespace euclide {

/// @brief Used to render a nodeItem
class NodeItemRenderer {
  public:
    NodeItemRenderer(const std::weak_ptr<NodeItemModel> t_model);

    void render();

  private:
    void drawRect(const std::shared_ptr<NodeItemModel>& t_nodeModel) const;

    void drawIOs(const std::shared_ptr<NodeItemModel>& t_nodeModel);

    static void drawIO(const ImVec2 t_position, const float t_ioRadius, const ImU32 t_color,
                       const ImU32 t_outlineColor);

    static std::vector<ImVec2> computePointsPositionsOnLine(const int t_numberOfPoints, const ImVec2 t_linePosition,
                                                            const float t_lineWidth);

  public:
    static constexpr float IO_RADIUS = 5.5f;                                ///< IO radius.
    static constexpr float SPACING = 3.0f;                                  ///< Spacing between slots and node.
    static constexpr ImU32 IO_OUTLINE_COLOR = IM_COL32(200, 200, 200, 255); ///< IO slot outline color.
    static constexpr ImU32 IO_COLOR = IM_COL32(170, 170, 170, 255);         ///< IO slot color

  private:
    std::weak_ptr<NodeItemModel> m_nodeModel;

    std::vector<ImVec2> m_inputIOPositions;  ///< Cached input slot positions.
    std::vector<ImVec2> m_outputIOPositions; ///< Cached output slot positions.

    friend NodeItemInteraction;
};

} // namespace euclide