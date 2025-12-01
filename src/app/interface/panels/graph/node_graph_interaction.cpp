#include "node_graph_interaction.h"

namespace butter {

std::optional<uint32_t> NodeGraphInteraction::getNodeAt(const std::weak_ptr<NodeGraph> t_graph,
                                                        const ImVec2& t_mousePosition) {
    auto graph = t_graph.lock();
    if (!graph)
        return std::nullopt;

    for (auto it = graph->drawNodesOrder.rbegin(); it != graph->drawNodesOrder.rend(); ++it) {
        auto nodeItem = graph->nodes[*it];
        if (nodeItem->isHovered()) {
            return *it;
        }
    }
    return std::nullopt;
}

std::optional<IOInfos> NodeGraphInteraction::getNodeIOAt(const std::weak_ptr<NodeGraph> t_graph,
                                                         const ImVec2& t_mousePosition) {
    auto graph = t_graph.lock();
    if (!graph)
        return std::nullopt;

    for (auto [id, nodeItem] : graph->nodes) {
        std::optional<uint32_t> inputIndex = nodeItem->inputIOHovered();
        if (inputIndex.has_value()) {
            return IOInfos{id, IOType::INPUT, inputIndex.value()};
        }

        std::optional<uint32_t> outputIndex = nodeItem->outputIOHovered();
        if (outputIndex.has_value()) {
            return IOInfos{id, IOType::OUTPUT, outputIndex.value()};
        }
    }

    return std::nullopt;
}

std::optional<size_t> NodeGraphInteraction::getIntersectedConnectionIndex(const std::weak_ptr<NodeGraph> t_graph,
                                                                          const ImVec2& t_startPos,
                                                                          const ImVec2& t_endPos) {
    auto graph = t_graph.lock();
    if (!graph)
        return std::nullopt;

    for (size_t i = 0; i < graph->connections.size(); ++i) {
        const auto& connection = graph->connections[i];
        if (lineIntersect(connection.sourcePosition(), connection.destinationPosition(), t_startPos, t_endPos)) {
            return i;
        }
    }

    return std::nullopt;
}

} // namespace butter