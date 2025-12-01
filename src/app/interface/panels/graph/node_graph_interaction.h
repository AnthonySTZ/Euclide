#pragma once

#include "node_graph.h"

namespace butter {

/// @brief Provides helper functions for querying and interacting with a NodeGraph.
/// @details This struct contains only static methods for determining which nodes, IO slots,
/// or connections are under the mouse, or intersected by a line. These functions do not modify
/// the graph directly, they only provide information for input handling and interaction.
struct NodeGraphInteraction {
    /// @brief Returns the ID of the node under the mouse, if any.
    /// @param t_graph Weak pointer to the NodeGraph to query.
    /// @param t_mousePosition Mouse position in graph coordinates.
    /// @return The ID of the node under the mouse, or std::nullopt if none is hovered.
    [[nodiscard]] static std::optional<uint32_t> getNodeAt(const std::weak_ptr<NodeGraph> t_graph,
                                                           const ImVec2& t_mousePosition);

    /// @brief Returns information about an IO slot (input or output) under the mouse, if any.
    /// @param t_graph Weak pointer to the NodeGraph to query.
    /// @param t_mousePosition Mouse position in graph coordinates.
    /// @return IOInfos struct with node ID, IO type, and index, or std::nullopt if none is hovered.
    [[nodiscard]] static std::optional<IOInfos> getNodeIOAt(const std::weak_ptr<NodeGraph> t_graph,
                                                            const ImVec2& t_mousePosition);

    /// @brief Returns the index of the first connection intersected by a line segment.
    /// @param t_graph Weak pointer to the NodeGraph to query.
    /// @param t_startPos Starting point of the line segment.
    /// @param t_endPos Ending point of the line segment.
    /// @return Index of the intersected connection, or std::nullopt if no intersection occurs.
    [[nodiscard]] static std::optional<size_t> getIntersectedConnectionIndex(const std::weak_ptr<NodeGraph> t_graph,
                                                                             const ImVec2& t_startPos,
                                                                             const ImVec2& t_endPos);
};

} // namespace butter
