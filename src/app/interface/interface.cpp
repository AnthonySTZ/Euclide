#include "interface.h"

namespace butter {

Interface::Interface(GLFWwindow *t_window)
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

}