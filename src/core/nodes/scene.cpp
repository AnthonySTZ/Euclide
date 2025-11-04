#include "scene.h"

namespace butter {

void Scene::addNode(const std::shared_ptr<Node>& t_node)
{
    const std::string nodeName = findName(t_node->name());
    m_nodes.emplace(nodeName, t_node);
    t_node->setName(nodeName);
}

std::string Scene::findName(const std::string& t_name) {
    if (m_nodes.find(t_name) == m_nodes.end()) return t_name;
    
    unsigned int index = 1;
    while (m_nodes.find(t_name + std::to_string(index)) != m_nodes.end()){
        index++;
    }
    return t_name + std::to_string(index);
}
    
}