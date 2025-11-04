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
    m_nodes.emplace(nodeName, t_node);
    t_node->setName(nodeName);
}

/**
 * @brief Remove a node from the scene based on its name
 * 
 * @param t_name The node name
 */
void Scene::removeNode(const std::string &t_name)
{
    m_nodes.erase(t_name);
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
    auto it = m_nodes.find(t_name);
    if (it == m_nodes.end()) return nullptr;
    return it->second;
}

/**
 * @brief Check if a name is already taken and return a new one that is not taken by adding a number at the end.
 * 
 * @param t_name The name to check
 * 
 * @return A not taken name, can return the `t_name` if it is not taken 
 */
std::string Scene::findName(const std::string& t_name) const {
    if (m_nodes.find(t_name) == m_nodes.end()) return t_name;
    
    unsigned int index = 1;
    while (m_nodes.find(t_name + std::to_string(index)) != m_nodes.end()){
        index++;
    }
    return t_name + std::to_string(index);
}
    
}