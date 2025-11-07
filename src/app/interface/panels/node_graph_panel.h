#pragma once

#include "panel.h"
#include "nodes/scene.h"
#include "node_item.h"

#include <set>

namespace butter {

class NodeGraph : public Panel {

public:
    NodeGraph(const std::shared_ptr<Scene>& t_scene);
    ~NodeGraph() = default;

    void draw() override;
    
private:
    void handleInputs();
    void handleCreateNode();
    void handleNodeInteractions();
    void leftMouseReleased();
    void handleNodeClicked();
    void leftMouseDown();
    void refreshHoveredNode();
    void handleDragGraph();
    void handleKeyInput();
    void removeSelectedNodes();
    void drawNodes();

    void clearSelection();

    void addToSelection(const std::shared_ptr<NodeItem> &t_nodeItem, const bool t_removeIfAlreadySelected = true);

    std::weak_ptr<Scene> m_scene;

    std::unordered_map<uint32_t, std::shared_ptr<NodeItem>> m_nodeItems;
    std::vector<uint32_t> m_drawOrder{};
    
    bool m_isNodeDrag = false;
    bool m_isGraphDrag = false;
    
    std::shared_ptr<NodeItem> m_nodeHovered = nullptr;
    std::set<std::shared_ptr<NodeItem>> m_selectedNodes{};
};

}