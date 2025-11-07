#pragma once

#include "node_item.h"

#include <memory>

namespace butter {

class ConnectionItem {

public:
    ConnectionItem() = default;

    void draw() const;

    void setSource(const std::shared_ptr<NodeItem>& t_sourceNode, const uint32_t t_sourceIndex);
    void setDestination(const std::shared_ptr<NodeItem> &t_destNode, const uint32_t t_destIndex);

private:
    std::weak_ptr<NodeItem> m_sourceNode = std::weak_ptr<NodeItem>();
    std::weak_ptr<NodeItem> m_destNode = std::weak_ptr<NodeItem>();

    uint32_t m_sourceIndex = 0;
    uint32_t m_destIndex = 0;

};


}