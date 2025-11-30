#pragma once

#include "node.h"

#include <vector>
#include <functional>
#include <memory>
#include <string>

namespace butter {

/// @brief Represents a single item in the node creation menu.
struct NodeMenuItem {
    /// @brief Display name of the node.
    std::string name;

    /// @brief Factory function to create the node.
    std::function<std::shared_ptr<Node>()> createNode;
};

/// @brief Provides information about all available node types and menu structure.
struct NodesInfo {
    /// @brief Returns the list of node categories and their corresponding menu items.
    /// @return Vector of pairs: category name and vector of NodeMenuItem.
    static const std::vector<std::pair<std::string, std::vector<NodeMenuItem>>>& getMenuItems();
};

} // namespace butter
