#include "parameters_renderer.h"

#include "interface/panels/parameters/fields/field_drawer.h"

namespace euclide {

ParametersRenderer::ParametersRenderer(const std::weak_ptr<Parameters> t_parameters) : m_parameters(t_parameters) {
}

void ParametersRenderer::draw() {
    pushStyle();
    drawParameters();
    popStyle();
}

void ParametersRenderer::drawParameters() {
    auto parameters = m_parameters.lock();
    if (!parameters)
        return;

    auto node = parameters->node().lock();
    if (!node)
        return;

    FieldDrawer drawer;
    drawTitleName(node->name());

    if (ImGui::BeginTable("params", 2)) {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 150.0f);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

        for (const auto& [name, field] : node->fields()) {
            const NodeFieldMetadata& meta = field->metadata();
            if (meta.shouldBeHidden()) {
                continue;
            }
            std::string displayName = meta.displayName.value_or(name);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(displayName.c_str());

            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-1);
            field->accept(name, drawer);
        }

        ImGui::EndTable();
    }
}

void ParametersRenderer::drawTitleName(const std::string& t_title) {
    ImGui::Text("Name: ");
    ImGui::SameLine();
    ImGui::Text(t_title.c_str());
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + TITLE_SPACING);
}

void ParametersRenderer::pushStyle() {
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(90, 90, 90, 255));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 70, 255));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(60, 60, 60, 255));

    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(90, 90, 90, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(70, 70, 70, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(60, 60, 60, 255));

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
}

void ParametersRenderer::popStyle() {
    ImGui::PopStyleColor(6);
    ImGui::PopStyleVar(1);
}

} // namespace euclide