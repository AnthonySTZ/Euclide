#pragma once

#include "node_item.h"
#include <memory>

namespace butter {

/// @brief Represents a connection between two nodes in the node editor.
class ConnectionItem {
  public:
    ConnectionItem() = default;

    /// @brief Draw the connection line between source and destination nodes.
    void draw();

    /// @brief Set the source node and its output index for this connection.
    /// @param t_sourceNode Weak pointer to the source NodeItem.
    /// @param t_sourceIndex Index of the output slot on the source node.
    void setSource(const std::weak_ptr<NodeItem> t_sourceNode, uint32_t t_sourceIndex) noexcept;

    /// @brief Set the destination node and its input index for this connection.
    /// @param t_destNode Weak pointer to the destination NodeItem.
    /// @param t_destIndex Index of the input slot on the destination node.
    void setDestination(const std::weak_ptr<NodeItem> t_destNode, uint32_t t_destIndex) noexcept;

    /// @brief Get the source node.
    [[nodiscard]] std::shared_ptr<NodeItem> sourceNode() const { return m_sourceNode.lock(); }

    /// @brief Get the destination node.
    [[nodiscard]] std::shared_ptr<NodeItem> destinationNode() const { return m_destNode.lock(); }

    /// @brief Get the source index.
    [[nodiscard]] uint32_t sourceIndex() const noexcept { return m_sourceIndex; }

    /// @brief Get the destination index.
    [[nodiscard]] uint32_t destinationIndex() const noexcept { return m_destIndex; }

    /// @brief Get the position of the source node's slot in UI coordinates.
    [[nodiscard]] ImVec2 sourcePosition() const { return m_start; }

    /// @brief Get the position of the destination node's slot in UI coordinates.
    [[nodiscard]] ImVec2 destinationPosition() const { return m_end; }

    /// @brief Remove the connection from the nodes and UI.
    void deleteConnection();

  private:
    std::weak_ptr<NodeItem> m_sourceNode; ///< Source node of the connection
    std::weak_ptr<NodeItem> m_destNode;   ///< Destination node of the connection

    uint32_t m_sourceIndex = 0; ///< Index of the source output slot
    uint32_t m_destIndex = 0;   ///< Index of the destination input slot

    ImVec2 m_start{0.0f, 0.0f}; ///< Start position of the connection line
    ImVec2 m_end{0.0f, 0.0f};   ///< End position of the connection line

    static constexpr ImU32 COLOR = IM_COL32(200, 200, 100, 255); ///< Default color of the connection
    static constexpr float THICKNESS = 2.0f;                     ///< Thickness of the connection line
};

} // namespace butter