#include "scene.h"

namespace butter {

/**
 * @brief Add a node to the scene and rename it if its name is already taken
 * 
 * @param t_node The node to add to the scene
 */
void Scene::addNode(const std::shared_ptr<Node>& t_node)
{
    const std::string nodeName = findName(t_node->name());
    const uint32_t id = findId();
    t_node->setName(nodeName);
    t_node->setId(id);
    m_nodesIds.emplace(nodeName, id);
    m_nodes.emplace(id, t_node);

    onNodeAdded.notify(id, t_node);

    t_node->onSetInput.subscribe([this](const uint32_t t_sourceId, const uint32_t t_sourceIndex, const uint32_t t_destId, const uint32_t t_destIndex) {
        onConnectionAdded.notify(t_sourceId, t_sourceIndex, t_destId, t_destIndex);
    });

    t_node->onRemoveInput.subscribe([this](const uint32_t t_sourceId, const uint32_t t_destId, const uint32_t t_destIndex) {
        onConnectionRemoved.notify(t_sourceId, t_destId, t_destIndex);
    });
}

/**
 * @brief Remove a node from the scene based on its name
 * 
 * @param t_name The node name
 */
void Scene::removeNode(const std::string &t_name)
{
    auto it_id = m_nodesIds.find(t_name);
    if (it_id == m_nodesIds.end()) return;
    m_nodesIds.erase(t_name);
    
    auto it_node = m_nodes.find(it_id->second);
    if (it_node == m_nodes.end()) return;
    m_nodes.erase(it_id->second);

    onNodeRemoved.notify(it_id->second);
}

/**
 * @brief Get a node in the scene based on its name
 * 
 * @param  t_name The node name
 * 
 * @return The node pointer if found, nullptr else
 */
std::shared_ptr<Node> Scene::node(const std::string &t_name) const noexcept
{
    auto it_id = m_nodesIds.find(t_name);
    if (it_id == m_nodesIds.end()) return nullptr;
    
    auto it_node = m_nodes.find(it_id->second);    
    if (it_node == m_nodes.end()) return nullptr;

    return it_node->second;
}

/**
 * @brief Check if a name is already taken and return a new one that is not taken by adding a number at the end.
 * 
 * @param t_name The name to check
 * 
 * @return A not taken name, can return the `t_name` if it is not taken 
 */
std::string Scene::findName(const std::string& t_name) const {
    if (m_nodesIds.find(t_name) == m_nodesIds.end()) return t_name;
    
    unsigned int index = 1;
    while (m_nodesIds.find(t_name + std::to_string(index)) != m_nodesIds.end()){
        index++;
    }
    return t_name + std::to_string(index);
}

uint32_t Scene::findId() const {

    uint32_t id = m_nodes.size();
    while (m_nodes.find(id) != m_nodes.end()) {
        id++;
    }
    return id;

}
    
}