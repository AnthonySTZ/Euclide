#include "viewport.h"

#include "nodes/primitive/grid.h"

namespace butter {

Viewport::Viewport(const std::weak_ptr<Scene> t_scene) : m_scene(t_scene), m_camera(std::make_shared<Camera>()) {
    Mesh grid{};
    Grid::createGrid(grid, GridSettings{.position = {0.0, 0.0, 0.0}, .size = {10.0, 10.0}, .divisions = {10, 10}});
    m_gridModel.updateWithMesh(grid);
    m_gridModel.showPrimitives = false;
    m_gridModel.showPoints = false;
    m_gridModel.showWireframe = true;

    if (auto scene = m_scene.lock()) {
        scene->onMeshUpdate.subscribe([this](std::shared_ptr<Mesh> t_mesh) { m_renderModel.updateWithMesh(*t_mesh); });
    }
}

} // namespace butter