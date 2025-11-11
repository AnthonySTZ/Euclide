#pragma once

#include "panel.h"
#include "nodes/scene.h"
#include "graph/node_item.h"
#include "graph/connection_item.h"

#include <set>

namespace butter {

enum class IOType {
    INPUT,
    OUTPUT
};

struct IOClickedInfos {
    std::shared_ptr<NodeItem> nodeItem = nullptr;
    IOType type = IOType::INPUT;
    int index = -1;
};

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

    void renderSelectedNode();

    IOClickedInfos isHoveredIO();
    void addConnection(const IOClickedInfos &t_infos);

    void clearCurrentConnection();

    void handleDragGraph();
    void handleKeyInput();
    void removeSelectedNodes();
    void drawConnections();
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
    std::vector<std::shared_ptr<ConnectionItem>> m_nodeConnections;

    std::unique_ptr<ConnectionItem> m_currentConnection;
};

}