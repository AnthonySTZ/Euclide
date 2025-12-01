#pragma once

#include "nodes/nodes_info.h"
#include "node_graph.h"

namespace butter {

/// @brief Handles the context menu for the NodeGraph.
/// @details Responsible for rendering the popup menu, search bar, and node creation items.
///          The menu allows adding nodes to the graph, filtering items by search text,
///          and styling the menu appearance.
class NodeGraphMenu {
  public:
    /// @brief Constructs a NodeGraphMenu for a given NodeGraph.
    /// @param t_nodeGraph Weak pointer to the NodeGraph that this menu will interact with.
    NodeGraphMenu(const std::weak_ptr<NodeGraph> t_nodeGraph) : m_graph(std::move(t_nodeGraph)){};

    ~NodeGraphMenu() = default;

    /// @brief Renders the context menu if it is open.
    /// @details Includes the search bar and node items, grouped by categories.
    void render();

  public:
    /// @brief Name of the ImGui popup for this context menu.
    static constexpr std::string_view CONTEXT_MENU_NAME = "NodeGraphContextMenu";

  private:
    /// @brief Pushes ImGui style settings for the popup.
    void beginStyle() const;

    /// @brief Pops ImGui style settings after drawing the popup.
    void endStyle() const;

    /// @brief Draws the search bar for filtering nodes.
    /// @details Automatically focuses the search bar the first time it is opened.
    void drawSearchBar();

    /// @brief Draws node items for a given category or filtered list.
    /// @param t_menu Name of the menu/category.
    /// @param t_items List of NodeMenuItem objects to draw.
    void drawItems(const std::string& t_menu, const std::vector<NodeMenuItem>& t_items);

    /// @brief Draws a single node item in the menu.
    /// @param t_item NodeMenuItem to draw.
    /// @details Adds the node to the NodeGraph if selected.
    void drawItem(const NodeMenuItem& t_item);

  private:
    std::weak_ptr<NodeGraph> m_graph; ///< Weak pointer to the NodeGraph being modified.

    std::string m_searchText;      ///< Current text in the search bar.
    bool m_focusSearchBar = false; ///< Whether the search bar should be focused next frame.

    static constexpr ImU32 SEARCH_BAR_COLOR = IM_COL32(50, 50, 50, 255); ///< Background color for the search bar.
};

} // namespace butter
