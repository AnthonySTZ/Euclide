#pragma once

#include "node_item.h"
#include "connection_item.h"
#include "nodes/scene.h"

#include <unordered_map>
#include <vector>
#include <unordered_set>

namespace euclide {

/// @brief Type of node input/output.
enum class IOType { INPUT, OUTPUT };

/// @brief Stores information about a node IO slot for connections.
/// @details Used when creating or querying connections.
struct IOInfos {
    uint32_t nodeId = 0;         ///< ID of the node.
    IOType type = IOType::INPUT; ///< Whether this is an input or output slot.
    uint32_t index = 0;          ///< Index of the IO slot within the node.
};

/// @brief Represents a graph of nodes and their connections.
/// @details Handles node creation, selection, connection management, and interaction
///          with the underlying Scene.
class NodeGraph {
  public:
    /// @brief Observer called when a node is selected.
    Observer<std::weak_ptr<Node>> onNodeSelected;

    /// @brief Constructs a NodeGraph from a Scene.
    /// @param t_scene Shared pointer to the Scene to bind to.
    NodeGraph(const std::shared_ptr<Scene> t_scene);
    ~NodeGraph() = default;

    /// @brief Adds a node to the graph.
    /// @param t_node Shared pointer to the node to add.
    void addNode(const std::shared_ptr<Node>& t_node);

    /// @brief Adds a connection between two IO slots.
    /// @param t_first First node IO information.
    /// @param t_second Second node IO information.
    void addConnection(const IOInfos& t_first, const IOInfos& t_second) const;

    /// @brief Removes a connection by its index.
    /// @param t_connectionIndex Index of the connection to remove.
    void removeConnection(const size_t t_connectionIndex);

    /// @brief Removes all selected nodes
    void removeSelectedNodes();

    /// @brief Retrieves a node item by ID.
    /// @param t_nodeId Node ID to query.
    /// @return Weak pointer to the NodeItem, empty if not found.
    [[nodiscard]] inline std::weak_ptr<NodeItem> getNode(const uint32_t t_nodeId) const noexcept {
        auto it = nodes.find(t_nodeId);
        if (it == nodes.end())
            return std::weak_ptr<NodeItem>();
        return it->second;
    }

    /// @brief Retrieves the set of currently selected node IDs.
    /// @return Set of selected node IDs.
    [[nodiscard]] inline std::unordered_set<uint32_t> selectedNodes() const noexcept { return m_selectedNodes; }

    /// @brief Adds a node to the selection.
    /// @param t_nodeId Node ID to select.
    /// @param t_removeIfAlreadySelected If true, deselects node if it was already selected.
    void addNodeToSelection(const uint32_t t_nodeId, const bool t_removeIfAlreadySelected);

    /// @brief Clears the current node selection.
    void clearSelection();

    /// @brief Triggers a render/cook on the last selected node.
    void renderSelectedNode();

  public:
    std::vector<ConnectionItem> connections;                       ///< List of all connections in the graph.
    std::unordered_map<uint32_t, std::shared_ptr<NodeItem>> nodes; ///< Node ID -> NodeItem mapping.
    std::vector<uint32_t> drawNodesOrder{};                        ///< Order in which nodes should be drawn.

  private:
    /// @brief Handles node addition events from the Scene.
    void onNodeAdded(const uint32_t t_nodeId, const std::shared_ptr<Node> t_node);

    /// @brief Handles node removal events from the Scene.
    void onNodeRemoved(const uint32_t t_nodeId);

    /// @brief Handles connection addition events from the Scene.
    void onConnectionAdded(const uint32_t t_sourceId, const uint32_t t_sourceIndex, const uint32_t t_destId,
                           const uint32_t t_destIndex);

    /// @brief Handles connection removal events from the Scene.
    void onConnectionRemoved(const uint32_t t_sourceId, const uint32_t t_destId, const uint32_t t_destIndex);

  private:
    std::unordered_set<uint32_t> m_selectedNodes; ///< IDs of currently selected nodes.
    std::weak_ptr<NodeItem> m_lastNodeSelected;   ///< Last selected node.
    std::weak_ptr<Scene> m_scene;                 ///< Weak pointer to the bound Scene.
};

} // namespace euclide
