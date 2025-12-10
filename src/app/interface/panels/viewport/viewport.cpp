#include "viewport.h"

#include "nodes/primitive/grid.h"

namespace euclide {

Viewport::Viewport(const std::weak_ptr<Scene> t_scene) : m_scene(t_scene), m_camera(std::make_shared<Camera>()) {
    Mesh grid{};
    Grid::createGrid(grid, GridSettings{.position = {0.0, 0.0, 0.0}, .size = {10.0, 10.0}, .divisions = {10, 10}});
    m_gridModel.updateWithMesh(grid);
    m_gridModel.showPrimitives = false;
    m_gridModel.showPoints = false;
    m_gridModel.showWireframe = true;
    m_gridModel.setEdgeColor(glm::vec3{0.5, 0.5, 0.5});

    if (auto scene = m_scene.lock()) {
        scene->onMeshUpdate.subscribe([this](std::shared_ptr<Mesh> t_mesh) { m_renderModel.updateWithMesh(*t_mesh); });
    }
}

void Viewport::retargetCamera() {
    auto scene = m_scene.lock();
    if (!scene)
        return;

    if (const auto node = scene->renderNode()) {
        float3 target = node->cook(0)->center();
        m_camera->updateTarget(glm::vec3{target[0], target[1], target[2]});
    }
}

} // namespace euclide