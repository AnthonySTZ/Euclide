#include "node.h"

#include <algorithm>

namespace butter {

Node::Node(const size_t t_nInputs, const size_t t_nOutputs)
{
    m_inputConnections.resize(t_nInputs, nullptr);
    m_outputConnections.resize(t_nOutputs, {});
    m_cachedMesh.resize(t_nOutputs, nullptr);
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
    if (auto sourceNode = t_sourceNode.lock()) {
        if (t_sourceIndex >= sourceNode->m_outputConnections.size()) return;
        deleteInputConnection(t_index);
        
        auto conn = std::make_shared<NodeConnection>();
        conn->sourceNode = t_sourceNode;
        conn->sourceIndex = t_sourceIndex;
        conn->destNode = shared_from_this();
        conn->destIndex = t_index;
        
        m_inputConnections[t_index] = conn;
        sourceNode->m_outputConnections[t_sourceIndex].push_back(conn);
        
        setDirty();
    }
}

/**
 * @brief Delete the input connection at given index, remove also the output connection from the sourceNode which the node was connected before
 * 
 * @param t_index The index to remove
 * 
 */
void Node::deleteInputConnection(const size_t t_index)
{
    if (t_index >= m_inputConnections.size()) return;

    auto conn = m_inputConnections[t_index];
    m_inputConnections[t_index] = nullptr;
    if (conn == nullptr) return;
    if (auto sourceNode = conn->sourceNode.lock()) {
        auto& sourceOutputConnections = sourceNode->m_outputConnections[conn->sourceIndex];
        sourceOutputConnections.erase(
            std::remove(sourceOutputConnections.begin(), sourceOutputConnections.end(), conn),
            sourceOutputConnections.end()
        );
    }
}

/**
 * @brief Set current node as dirty and propagate to its outputs nodes 
 * 
 */
void Node::setDirty()
{
    std::fill(m_isDirty.begin(), m_isDirty.end(), true);
    for (auto& outputConnections: m_outputConnections) {
        for (auto& conn: outputConnections) {
            if (conn == nullptr) continue;
            if(auto destNode = conn->destNode.lock()) {
                destNode->setDirty();
            }
        }
    }
}

/**
 * @brief Get the input `Node` at the given index 
 * 
 * @param t_index The input index to get the connection
 * 
 * @return The NodeConnection at the given index
 * 
 */
std::shared_ptr<NodeConnection> Node::getInputConnection(const size_t t_index)
{
    return m_inputConnections[t_index];
}

std::vector<std::shared_ptr<NodeConnection>> Node::getOutputConnections(const size_t t_index)
{
    return m_outputConnections[t_index];
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