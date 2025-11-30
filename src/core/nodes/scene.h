#pragma once

#include "node.h"
#include "geometry/mesh.h"

#include <unordered_map>
#include <string>

namespace butter {

/// @brief Represents a collection of nodes and manages their connections and execution.
class Scene {
  public:
    /// @brief Triggered when a node is added.
    Observer<uint32_t, std::shared_ptr<Node>> onNodeAdded;

    /// @brief Triggered when a node is removed.
    Observer<uint32_t> onNodeRemoved;

    /// @brief Triggered when a connection between nodes is added.
    /// @param source_id ID of source node.
    /// @param source_index Output index of source node.
    /// @param dest_id ID of destination node.
    /// @param dest_index Input index of destination node.
    Observer<uint32_t, uint32_t, uint32_t, uint32_t> onConnectionAdded;

    /// @brief Triggered when a connection between nodes is removed.
    /// @param source_id ID of source node.
    /// @param dest_id ID of destination node.
    /// @param dest_index Input index of destination node.
    Observer<uint32_t, uint32_t, uint32_t> onConnectionRemoved;

    /// @brief Triggered when the mesh of the current render node is updated.
    Observer<std::shared_ptr<Mesh>> onMeshUpdate;

    /// @brief Returns the number of nodes in the scene.
    /// @return Number of nodes.
    [[nodiscard]] size_t size() const noexcept { return m_nodes.size(); }

    /// @brief Adds a node to the scene.
    /// @param t_node Node to be added.
    void addNode(const std::shared_ptr<Node>& t_node);

    /// @brief Sets a node as the current render node and updates its mesh.
    /// @param t_node Node to render.
    /// @param t_index Output index of the node to cook (default is 0).
    void cookNode(const std::shared_ptr<Node>& t_node, const uint32_t t_index = 0);

    /// @brief Removes a node by name from the scene.
    /// @param t_name Name of the node to remove.
    void removeNode(const std::string& t_name);

    /// @brief Retrieves a node by its name.
    /// @param t_name Name of the node.
    /// @return Shared pointer to the node, or nullptr if not found.
    [[nodiscard]] std::shared_ptr<Node> node(const std::string& t_name) const noexcept;

    /// @brief Returns the current render node.
    /// @return Shared pointer to the current render node, or nullptr if none.
    [[nodiscard]] std::shared_ptr<Node> renderNode() const { return m_currentRenderNode.lock(); }

  private:
    /// @brief Generates a unique name for a node based on the requested name.
    /// @param t_name Requested name.
    /// @return Unique name.
    [[nodiscard]] std::string findName(const std::string& t_name) const;

    /// @brief Generates a unique ID for a node.
    /// @return Unique ID.
    [[nodiscard]] uint32_t findId() const;

    std::unordered_map<std::uint32_t, std::shared_ptr<Node>> m_nodes; ///< Map of node IDs to node objects.
    std::unordered_map<std::string, std::uint32_t> m_nodesIds;        ///< Map of node names to node IDs.

    std::weak_ptr<Node> m_currentRenderNode; ///< Currently active render node.
};

} // namespace butter
