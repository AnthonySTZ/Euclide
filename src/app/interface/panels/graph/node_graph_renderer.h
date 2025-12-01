#pragma once

#include "node_graph.h"

#include "cutting_line.h"
#include "interface/utils/fixed_queue.h"

namespace butter {

class NodeGraphRenderer {
  public:
    NodeGraphRenderer(const std::weak_ptr<NodeGraph> t_nodeGraph) : m_graph(std::move(t_nodeGraph)){};

    void render() const;

    void startConnection(const IOInfos& t_infos);
    void endConnection();

    inline void addCuttingLine(const ImVec2& t_startPos, const ImVec2& t_endPos) {
        m_cuttingsLines.emplace(CuttingLine{t_startPos, t_endPos});
    }
    inline void clearCuttingLines() { m_cuttingsLines.clear(); }

  private:
    void drawNodes() const;
    void drawConnections() const;
    void drawCuttingLines() const;

  private:
    std::weak_ptr<NodeGraph> m_graph;

    std::unique_ptr<ConnectionItem> m_currentConnection;
    FixedQueue<CuttingLine> m_cuttingsLines{200};
};

} // namespace butter