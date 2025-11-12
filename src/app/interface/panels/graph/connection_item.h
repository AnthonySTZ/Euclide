#pragma once

#include "node_item.h"

#include <memory>

namespace butter {

class ConnectionItem {

public:
    ConnectionItem() = default;

    void draw();

    void setSource(const std::shared_ptr<NodeItem>& t_sourceNode, const uint32_t t_sourceIndex);
    void setDestination(const std::shared_ptr<NodeItem> &t_destNode, const uint32_t t_destIndex);

    std::shared_ptr<NodeItem> sourceNode() { return m_sourceNode.lock(); }
    std::shared_ptr<NodeItem> destinationNode() { return m_destNode.lock(); }

    uint32_t sourceIndex() const noexcept { return m_sourceIndex; }
    uint32_t destinationIndex() const noexcept { return m_destIndex; }

    ImVec2 sourcePosition() const { return m_start; }
    ImVec2 destinationPosition() const { return m_end; }

    void deleteConnection();

private:
    std::weak_ptr<NodeItem> m_sourceNode = std::weak_ptr<NodeItem>();
    std::weak_ptr<NodeItem> m_destNode = std::weak_ptr<NodeItem>();

    uint32_t m_sourceIndex = 0;
    uint32_t m_destIndex = 0;

    ImVec2 m_start;
    ImVec2 m_end;

    static constexpr ImU32 s_color = IM_COL32(200, 200, 100, 255);
    static constexpr float s_thickness = 2.0f; 

};


}