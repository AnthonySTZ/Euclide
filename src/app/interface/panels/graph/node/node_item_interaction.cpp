#include "node_item_interaction.h"

namespace euclide {

ImVec2 NodeItemInteraction::getInputIOPosition(const std::shared_ptr<NodeItem> t_nodeItem, const uint32_t t_index) {
    const auto& renderer = t_nodeItem->renderer();
    return renderer.m_inputIOPositions[t_index];
}

ImVec2 NodeItemInteraction::getOutputIOPosition(const std::shared_ptr<NodeItem> t_nodeItem, const uint32_t t_index) {
    const auto& renderer = t_nodeItem->renderer();
    return renderer.m_outputIOPositions[t_index];
}

} // namespace euclide