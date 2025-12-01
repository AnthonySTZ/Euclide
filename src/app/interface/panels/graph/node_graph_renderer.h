#pragma once

#include "node_graph.h"

namespace butter {

class NodeGraphRenderer {
  public:
    NodeGraphRenderer(const std::weak_ptr<NodeGraph> t_nodeGraph) : m_graph(std::move(t_nodeGraph)){};

    void render() const;
    [[nodiscard]] std::optional<uint32_t> getNodeAt(ImVec2 t_mousePosition) const;
    [[nodiscard]] std::optional<IOInfos> getNodeIOAt(ImVec2 t_mousePosition) const;

    void startConnection(const IOInfos& t_infos);
    void endConnection();

  private:
    void drawNodes() const;
    void drawConnections() const;

  private:
    std::weak_ptr<NodeGraph> m_graph;

    std::unique_ptr<ConnectionItem> m_currentConnection;
};

} // namespace butter