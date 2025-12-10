#pragma once

#include "node_item.h"

namespace euclide {

/// @brief Type of node input/output.
enum class IOType { INPUT, OUTPUT };

/// @brief Stores information about a node IO slot for connections.
/// @details Used when creating or querying connections.
struct IOInfos {
    uint32_t nodeId = 0;         ///< ID of the node.
    IOType type = IOType::INPUT; ///< Whether this is an input or output slot.
    uint32_t index = 0;          ///< Index of the IO slot within the node.
    ImVec2 position{0, 0};       ///< Position the IO slot within the node.
};

class NodeItemInteraction {
  public:
    /// @brief Get the position of a specific input slot.
    /// @param t_nodeItem Shared ptr of the node item.
    /// @param t_index Index of the input slot.
    /// @return Position of the input slot in UI coordinates.
    [[nodiscard]] static ImVec2 getInputIOPosition(const std::shared_ptr<NodeItem> t_nodeItem, const uint32_t t_index);

    /// @brief Get the position of a specific output slot.
    /// @param t_nodeItem Shared ptr of the node item.
    /// @param t_index Index of the output slot.
    /// @return Position of the output slot in UI coordinates.
    [[nodiscard]] static ImVec2 getOutputIOPosition(const std::shared_ptr<NodeItem> t_nodeItem, const uint32_t t_index);

  private:
};

} // namespace euclide