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

bool NodeItemInteraction::isNodeHovered(const std::shared_ptr<NodeItem> t_nodeItem) {
    auto model = t_nodeItem->model();
    ImGuiIO& io = ImGui::GetIO();

    const ImVec2 position = model->m_position;
    const ImVec2 size = model->m_size;
    const ImVec2 nodeEnd = position + size;

    return io.MousePos.x >= position.x && io.MousePos.x <= nodeEnd.x && io.MousePos.y >= position.y &&
           io.MousePos.y <= nodeEnd.y;
}

std::optional<IOInfos> NodeItemInteraction::getIOAt(const std::shared_ptr<NodeItem> t_nodeItem, const ImVec2 t_position,
                                                    const float t_thresholdRadius) {
    auto renderer = t_nodeItem->renderer();
    auto model = t_nodeItem->model();

    for (uint32_t i = 0; i < renderer.m_inputIOPositions.size(); ++i) {
        const ImVec2 diff = renderer.m_inputIOPositions[i] - t_position;
        const float dist2 = diff.x * diff.x + diff.y * diff.y;
        if (dist2 <= t_thresholdRadius)
            return IOInfos{model->node()->id(), IOType::INPUT, i, renderer.m_inputIOPositions[i]};
    }

    for (uint32_t i = 0; i < renderer.m_outputIOPositions.size(); ++i) {
        const ImVec2 diff = renderer.m_outputIOPositions[i] - t_position;
        const float dist2 = diff.x * diff.x + diff.y * diff.y;
        if (dist2 <= t_thresholdRadius)
            return IOInfos{model->node()->id(), IOType::OUTPUT, i, renderer.m_outputIOPositions[i]};
    }

    return std::nullopt;
}

} // namespace euclide