#include "node.h"

namespace butter {

void Node::setInput(const size_t t_index, const std::weak_ptr<Node> &t_sourceNode, const size_t t_sourceIndex)
{
    if(t_index >= m_inputConnections.size()) return;

    auto conn = std::make_shared<NodeConnection>();
    conn->sourceNode = t_sourceNode;
    conn->sourceIndex = t_sourceIndex;
    conn->destNode = shared_from_this();
    conn->destIndex = t_index;

    m_inputConnections[t_index] = conn;
}
    
}