#include "viewport.h"

#include "nodes/primitive/grid.h"

namespace butter {
    
Viewport::Viewport(const std::shared_ptr<Scene> &t_scene)
    : m_scene(t_scene), m_renderer(std::make_unique<Renderer>()), m_camera(std::make_shared<Camera>())
{

    Mesh grid{};
    Grid::createGrid(grid, GridSettings{
        .position = {0.0, 0.0, 0.0}, 
        .size = {10.0, 10.0},
        .divisions = {10, 10}
    });
    m_gridModel.updateWithMesh(grid);
    m_gridModel.showPrimitives = false;
    m_gridModel.showPoints = false;
    m_gridModel.showWireframe = true;

    if (auto scene = m_scene.lock()) {
        scene->onMeshUpdate.subscribe(
            [this](std::shared_ptr<Mesh> t_mesh) {
                m_renderModel.updateWithMesh(*t_mesh);
            }
        );
    }

    m_renderer->setCamera(m_camera);
}

void Viewport::draw()
{
    beginTab("Viewport", m_padding);

    m_windowPosition = ImGui::GetCursorPos();
    checkForResize();

    handleMouse();
    handleKeys();

    drawRender();
    drawInfos();
    
    endTab();
}

void Viewport::handleMouse() {

    if (m_isItemHovered) {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            m_isLeftClicked = true;
        }
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle)) {
            m_isMiddleClicked = true;
        }
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            m_isRightClicked = true;
        }
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        m_isLeftClicked = false;
    }
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle)) {
        m_isMiddleClicked = false;
    }
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
        m_isRightClicked = false;
    }

    moveCamera();
    
}

void Viewport::handleKeys() {
    if (!m_isItemHovered) return;

    if (ImGui::IsKeyPressed(ImGuiKey_P)) m_renderModel.tooglePrimitives();
    if (ImGui::IsKeyPressed(ImGuiKey_W)) m_renderModel.toogleWireframe();
    if (ImGui::IsKeyPressed(ImGuiKey_V)) m_renderModel.tooglePoints();
    if (ImGui::IsKeyPressed(ImGuiKey_F)) retargetCamera();
}

void Viewport::retargetCamera() {
    if (const auto scene = m_scene.lock()) {
        if (const auto node = scene->renderNode()) {
            float3 target = node->cook(0)->center();
            m_camera->updateTarget(glm::vec3{target[0], target[1], target[2]});
        }
    }
}

void Viewport::moveCamera() {

    if (m_isLeftClicked) {
        ImVec2 dragDelta = ImGui::GetIO().MouseDelta;
        ImGui::SetMouseCursor(ImGuiMouseSource_Pen);
        m_camera->orbit(-dragDelta.x, -dragDelta.y);
    }

    if (m_isMiddleClicked) {
        ImVec2 dragDelta = ImGui::GetIO().MouseDelta;
        ImGui::SetMouseCursor(ImGuiMouseSource_Pen);
        m_camera->pan(-dragDelta.x, dragDelta.y);
    }

    if (m_isRightClicked) {
        ImVec2 dragDelta = ImGui::GetIO().MouseDelta;
        ImGui::SetMouseCursor(ImGuiMouseSource_Pen);
        m_camera->dolly(-dragDelta.y);
    }
}

void Viewport::drawRender() {
    m_renderer->beginFrame(m_viewportWidth, m_viewportHeight);
    m_renderer->clearFrame();

    m_renderer->draw(m_gridModel);
    m_renderer->draw(m_renderModel);
    
    m_renderer->endFrame(m_viewportWidth, m_viewportHeight);

    ImTextureID textureID = m_renderer->getRenderTexture();
    ImGui::Image(textureID, ImVec2((float)m_viewportWidth, (float)m_viewportHeight));

    m_isItemHovered = ImGui::IsItemHovered();
}

void Viewport::checkForResize() {
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    
    if (m_viewportWidth != viewportSize.x || m_viewportHeight != viewportSize.y) {
        m_viewportWidth = viewportSize.x;
        m_viewportHeight = viewportSize.y;
        m_renderer->resizeFrameBuffer(m_viewportWidth, m_viewportHeight);
    }
}

void Viewport::drawInfos() const
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImGuiIO& io = ImGui::GetIO();
    std::string fpsText = "Fps: " + std::to_string(static_cast<int>(io.Framerate));
    drawList->AddText(m_windowPosition + ImVec2(10, 10), IM_COL32(255, 255, 255, 255), fpsText.c_str());

    drawList->AddText(m_windowPosition + ImVec2(10, 35), IM_COL32(255, 255, 255, 255), "P: Toogle Primitives");
    drawList->AddText(m_windowPosition + ImVec2(10, 60), IM_COL32(255, 255, 255, 255), "W: Toogle Wireframe");
    drawList->AddText(m_windowPosition + ImVec2(10, 85), IM_COL32(255, 255, 255, 255), "V: Toogle Points");

}

}