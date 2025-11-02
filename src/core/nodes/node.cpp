#include "node.h"

namespace butter {

Node::Node(const size_t t_nInputs, const size_t t_nOutputs)
: m_inputConnections(t_nInputs),
    m_outputConnections(t_nOutputs),
    m_cachedMesh(t_nOutputs)
{
    m_isDirty.resize(t_nOutputs, true);
}

/**
 * @brief Create a node connection between two nodes
 * 
 * @param t_index The current input index to be connected from a source node
 * @param t_sourceNode The source node
 * @param t_sourceIndex The output index of the source node
 * 
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

/**
 * @brief Cook the node output and compute all needed source nodes
 *
 * @param t_index The output index we want to compute
 *
 * @return The computed `shared_ptr<Mesh>`
 *
 */
std::shared_ptr<Mesh> Node::cook(const size_t t_index)
{
    if(t_index >= m_outputConnections.size()) return nullptr;
    if (!m_isDirty[t_index]) return m_cachedMesh[t_index];

    std::vector<std::shared_ptr<Mesh>> inputMeshes;
    for (auto& conn: m_inputConnections){
        if (conn == nullptr) {
            inputMeshes.push_back(nullptr);
            continue;
        }
        if (auto sourceNode = conn->sourceNode.lock()) {
            inputMeshes.push_back(sourceNode->cook(conn->sourceIndex));
        }
    }

    m_cachedMesh[t_index] = compute(t_index, inputMeshes);
    m_isDirty[t_index] = false;
    return m_cachedMesh[t_index];
}
    
}