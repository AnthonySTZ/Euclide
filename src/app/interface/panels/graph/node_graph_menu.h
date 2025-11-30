#pragma once

#include "node_graph.h"

namespace butter {

class NodeGraphMenu {
  public:
    NodeGraphMenu(const std::weak_ptr<NodeGraph> t_nodeGraph) : m_graph(std::move(t_nodeGraph)){};
    ~NodeGraphMenu() = default;

    void render();

  private:
    void beginStyle() const;
    void endStyle() const;

    void drawSearchBar();

  private:
    std::weak_ptr<NodeGraph> m_graph;

    std::string m_searchText;
    bool m_focusSearchBar = false;

    static constexpr ImU32 SEARCH_BAR_COLOR = IM_COL32(50, 50, 50, 255);
};

} // namespace butter