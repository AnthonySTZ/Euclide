#pragma once 

#include "nodes/scene.h"
#include "panel.h"

#include <memory>

namespace butter {

class Viewport : public Panel {

public:
    Viewport(const std::shared_ptr<Scene>& t_scene);
    ~Viewport() = default;

private:
    std::weak_ptr<Scene> m_scene;


};


}