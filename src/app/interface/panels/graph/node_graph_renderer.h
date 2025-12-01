#pragma once

#include "node_graph.h"
#include "cutting_line.h"
#include "interface/utils/fixed_queue.h"

namespace butter {

/// @brief Responsible for rendering nodes, connections, and temporary interactions in a NodeGraph.
/// @details Handles drawing of nodes, their connections, cutting lines, and in-progress connections.
///          It interacts with NodeGraph via a weak pointer to prevent ownership cycles.
class NodeGraphRenderer {
  public:
    /// @brief Constructs a renderer for a given NodeGraph.
    /// @param t_nodeGraph Weak pointer to the NodeGraph to render.
    explicit NodeGraphRenderer(const std::weak_ptr<NodeGraph> t_nodeGraph) : m_graph(std::move(t_nodeGraph)){};

    /// @brief Renders all elements of the graph: nodes, connections, and cutting lines.
    void render() const;

    /// @brief Begins a temporary connection (dragging a connection from a node slot).
    /// @param t_infos Information about the node and IO slot where the connection starts.
    void startConnection(const IOInfos& t_infos);

    /// @brief Ends the temporary connection, discarding any in-progress connection.
    void endConnection();

    /// @brief Adds a cutting line between two points.
    /// @param t_startPos Start position of the line.
    /// @param t_endPos End position of the line.
    inline void addCuttingLine(const ImVec2& t_startPos, const ImVec2& t_endPos) {
        m_cuttingsLines.emplace(CuttingLine{t_startPos, t_endPos});
    }

    /// @brief Clears all currently drawn cutting lines.
    inline void clearCuttingLines() { m_cuttingsLines.clear(); }

    /// @brief Draw a box selection
    /// @param t_startPos Start position of the box
    /// @param t_endPos End position of the box
    void drawBoxSelection(const ImVec2& t_startPos, const ImVec2& t_endPos) const;

  private:
    /// @brief Draws all nodes in the graph.
    void drawNodes() const;

    /// @brief Draws all permanent connections and any in-progress connection.
    void drawConnections() const;

    /// @brief Draws all temporary cutting lines.
    void drawCuttingLines() const;

  private:
    std::weak_ptr<NodeGraph> m_graph; ///< Weak pointer to the NodeGraph being rendered.

    std::unique_ptr<ConnectionItem> m_currentConnection; ///< Currently active connection being dragged.
    FixedQueue<CuttingLine> m_cuttingsLines{200};        ///< Temporary cutting lines for visual feedback.

    static constexpr ImU32 SELECTION_BOX_COLOR = IM_COL32(0, 154, 255, 80);
    static constexpr float SELECTION_BOX_ROUNDING = 2.0f;
};

} // namespace butter
