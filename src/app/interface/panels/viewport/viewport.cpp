#include "viewport.h"

#include "nodes/primitive/grid.h"

namespace euclide {

Viewport::Viewport(const std::weak_ptr<Scene> t_scene) : m_scene(t_scene), m_camera(std::make_shared<Camera>()) {
    setViewportModels();
    if (auto scene = m_scene.lock()) {
        scene->onMeshUpdate.subscribe([this](std::shared_ptr<Mesh> t_mesh) { m_renderModel.updateWithMesh(*t_mesh); });
    }
}

void Viewport::setViewportModels() {
    m_viewportModels.resize(2);

    Mesh grid{};
    Grid::createGrid(grid, GridSettings{.position = {0.0, 0.0, 0.0}, .size = {500.0, 500.0}, .divisions = {500, 500}});
    m_viewportModels[0].updateWithMesh(grid);
    m_viewportModels[0].showPrimitives = false;
    m_viewportModels[0].showPoints = false;
    m_viewportModels[0].showWireframe = true;
    m_viewportModels[0].edgesLineWidth = 0.5f;
    m_viewportModels[0].setEdgeColor(glm::vec3{0.38, 0.38, 0.38});

    Mesh grid_2{};
    Grid::createGrid(grid_2, GridSettings{.position = {0.0, 0.0, 0.0}, .size = {500.0, 500.0}, .divisions = {2, 2}});
    m_viewportModels[1].updateWithMesh(grid_2);
    m_viewportModels[1].showPrimitives = false;
    m_viewportModels[1].showPoints = false;
    m_viewportModels[1].showWireframe = true;
    m_viewportModels[1].edgesLineWidth = 1.3f;
    m_viewportModels[1].setEdgeColor(glm::vec3{0.45, 0.45, 0.45});
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