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

} // namespace euclide