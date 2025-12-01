#pragma once

#include "node_graph.h"

namespace butter {

struct NodeGraphInteraction {
    [[nodiscard]] static std::optional<uint32_t> getNodeAt(const std::weak_ptr<NodeGraph> t_graph,
                                                           const ImVec2& t_mousePosition);

    [[nodiscard]] static std::optional<IOInfos> getNodeIOAt(const std::weak_ptr<NodeGraph> t_graph,
                                                            const ImVec2& t_mousePosition);

    [[nodiscard]] static std::optional<size_t> getIntersectedConnectionIndex(const std::weak_ptr<NodeGraph> t_graph,
                                                                             const ImVec2& t_startPos,
                                                                             const ImVec2& t_endPos);
};

} // namespace butter