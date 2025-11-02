#include "scene.h"

namespace butter {

void Scene::addNode(const std::shared_ptr<Node>& t_node)
{
    m_nodes.push_back(t_node);
}
    
}