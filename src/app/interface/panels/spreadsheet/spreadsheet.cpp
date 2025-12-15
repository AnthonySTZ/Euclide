#include "spreadsheet.h"

namespace euclide {

Spreadsheet::Spreadsheet(std::weak_ptr<Scene> t_scene) {
    if (auto scene = t_scene.lock()) {
        scene->onMeshUpdate.subscribe([this](std::weak_ptr<Mesh> t_mesh) { m_mesh = t_mesh; });
    }
}

} // namespace euclide