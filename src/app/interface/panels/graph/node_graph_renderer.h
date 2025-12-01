#pragma once

#include "node_graph.h"

namespace butter {

enum class IOType { INPUT, OUTPUT };

struct IOClickedInfos {
    uint32_t nodeId = 0;
    IOType type = IOType::INPUT;
    uint32_t index = 0;
};

class NodeGraphRenderer {
  public:
    NodeGraphRenderer(const std::weak_ptr<NodeGraph> t_nodeGraph) : m_graph(std::move(t_nodeGraph)){};

    void render() const;
    [[nodiscard]] std::optional<uint32_t> getNodeAt(ImVec2 t_mousePosition) const;
    [[nodiscard]] std::optional<IOClickedInfos> getNodeIOAt(ImVec2 t_mousePosition) const;

  private:
    void drawNodes() const;
    void drawConnections() const;

  private:
    std::weak_ptr<NodeGraph> m_graph;
};

} // namespace butter