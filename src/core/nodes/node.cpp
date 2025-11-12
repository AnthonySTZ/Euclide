#include "node.h"

#include <algorithm>

namespace butter {

Node::Node(const size_t t_nInputs, const size_t t_nOutputs, const std::string& t_name)
{
    m_inputConnections.resize(t_nInputs, nullptr);
    m_outputConnections.resize(t_nOutputs, {});
    m_cachedMesh.resize(t_nOutputs, nullptr);
    m_isDirty = true;
    m_name = t_name;
}

/**
 * @brief Create a node connection between two nodes
 * 
 * @param t_index The current input index to be connected from a source node
 * @param t_sourceNode The source node
 * @param t_sourceIndex The output index of the source node
 * 
 */
void Node::setInput(const size_t t_index, const std::shared_ptr<Node> &t_sourceNode, const size_t t_sourceIndex)
{
    auto this_shared = shared_from_this();
    if(t_index >= m_inputConnections.size()) return;
    if(t_sourceNode == this_shared) return;
    if (t_sourceIndex >= t_sourceNode->m_outputConnections.size()) return;
    if (t_sourceNode->isInInputsHierarchy(this_shared)) return;

    deleteInputConnection(t_index);
    
    auto conn = std::make_shared<NodeConnection>(
        t_sourceIndex,
        t_index,
        t_sourceNode,
        this_shared
    );
    
    m_inputConnections[t_index] = conn;
    t_sourceNode->m_outputConnections[t_sourceIndex].push_back(conn);
    
    setDirty();
    onSetInput.notify(t_sourceNode->id(), t_sourceIndex, m_id, t_index);
}

bool Node::isInInputsHierarchy(const std::shared_ptr<Node> t_node) const noexcept {
    for (const auto& conn: m_inputConnections) {
        if (conn == nullptr) continue;
        if (const auto sourceNode = conn->sourceNode.lock()) {
            if (sourceNode == t_node) return true;
            if (sourceNode->isInInputsHierarchy(t_node)) return true; // recursive
        }
    }

    return false;
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
        onRemoveInput.notify(sourceNode->id(), m_id, t_index);
    }
}

/**
 * @brief Set current node as dirty and propagate to its outputs nodes 
 * 
 */
void Node::setDirty()
{
    m_isDirty = true;
    for (auto& outputConnections: m_outputConnections) {
        for (auto& conn: outputConnections) {
            if (conn == nullptr) continue;
            if(auto destNode = conn->destNode.lock()) {
                destNode->setDirty();
            }
        }
    }
    onDirty.notify(shared_from_this());
}

void Node::addField(const std::string &t_name, std::shared_ptr<NodeFieldBase> t_field)
{
    t_field->onValueChanged.subscribe(
        [this]() {
            setDirty();
        }
    );
    m_fields.emplace(t_name, t_field);
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
    if (!m_isDirty) return m_cachedMesh[t_index];

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
    m_isDirty = false;
    return m_cachedMesh[t_index];
}
    
}