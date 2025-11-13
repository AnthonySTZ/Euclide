#include "interface.h"

namespace butter {

Interface::Interface(GLFWwindow *t_window, const std::shared_ptr<Scene>& t_scene)
    : m_scene(t_scene)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(t_window, true);
    ImGui_ImplOpenGL3_Init();

    m_font = io.Fonts->AddFontFromFileTTF("assets/fonts/Inter/Inter.ttf", 18.0f);
    if (!m_font) {
        std::cerr << "Failed to load font!" << std::endl;
    }
}

Interface::~Interface()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Interface::createDockSpace() const {

    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    window_flags |=
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("MainDockSpaceWindow", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("GooZDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    ImGui::End();
}

void Interface::addPanel(std::shared_ptr<Panel> t_panel) {
    m_panels.emplace_back(t_panel);
}

void Interface::addNodeGraph()
{
    m_nodeGraph = std::make_shared<NodeGraph>(scene());
    m_panels.push_back(m_nodeGraph);
}

void Interface::addViewport()
{
    m_viewport = std::make_shared<Viewport>(scene());
    m_panels.push_back(m_viewport);
}

void Interface::addParameters()
{
    m_parameters = std::make_shared<Parameters>(m_nodeGraph);
    m_panels.push_back(m_parameters);
}

void Interface::draw() const {
    beginFrame();
    createDockSpace();

    for (auto &panel : m_panels) {
        panel->draw();
    }

    clearFrame();
    renderFrame();
}

void Interface::beginFrame() const {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::PushFont(m_font);
}

void Interface::renderFrame() const {
    ImGui::PopFont();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void Interface::clearFrame() const {
    glClearColor(s_bgColor.r, s_bgColor.g, s_bgColor.b, s_bgColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

}