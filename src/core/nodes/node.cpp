#include "node.h"

namespace butter {

/**
 * @brief Create a node connection between two nodes
 * 
 * @param t_index The current input index to be connected from a source node
 * @param t_sourceNode The source node
 * @param t_sourceIndex The output index of the source node
 */
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