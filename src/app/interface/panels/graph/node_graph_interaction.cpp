#include "node_graph_interaction.h"

namespace euclide {

std::optional<uint32_t> NodeGraphInteraction::getNodeAt(const std::weak_ptr<NodeGraph> t_graph,
                                                        const ImVec2& t_mousePosition) {
    auto graph = t_graph.lock();
    if (!graph)
        return std::nullopt;

    for (auto it = graph->drawNodesOrder.rbegin(); it != graph->drawNodesOrder.rend(); ++it) {
        auto nodeItem = graph->nodes[*it];
        if (NodeItemInteraction::isNodeHovered(nodeItem)) {
            return *it;
        }
    }
    return std::nullopt;
}

std::optional<IOInfos> NodeGraphInteraction::getNodeIOAt(const std::weak_ptr<NodeGraph> t_graph,
                                                         const ImVec2& t_mousePosition, const float t_thresholdRadius) {
    auto graph = t_graph.lock();
    if (!graph)
        return std::nullopt;

    for (auto [_, nodeItem] : graph->nodes) {
        std::optional<IOInfos> io = NodeItemInteraction::getIOAt(nodeItem, t_mousePosition, t_thresholdRadius);
        if (io.has_value())
            return io;
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

std::vector<uint32_t> NodeGraphInteraction::getNodesInRect(const std::weak_ptr<NodeGraph> t_graph,
                                                           const ImVec2& t_startPos, const ImVec2& t_endPos) {
    auto graph = t_graph.lock();
    if (!graph)
        return {};

    const float minX = std::min(t_startPos.x, t_endPos.x);
    const float maxX = std::max(t_startPos.x, t_endPos.x);

    const float minY = std::min(t_startPos.y, t_endPos.y);
    const float maxY = std::max(t_startPos.y, t_endPos.y);

    std::vector<uint32_t> nodes;
    for (auto [id, nodeItem] : graph->nodes) {
        auto model = nodeItem->model();
        const ImVec2 nodePosition = model->position();
        const ImVec2 nodeSize = model->size();
        if (nodePosition.x + nodeSize.x > minX && nodePosition.x < maxX && nodePosition.y + nodeSize.y > minY &&
            nodePosition.y < maxY) {
            nodes.push_back(id);
        }
    }

    return nodes;
}

} // namespace euclide