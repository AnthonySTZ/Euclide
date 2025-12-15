#include "spreadsheet_renderer.h"

namespace euclide {

SpreadsheetRenderer::SpreadsheetRenderer(const std::weak_ptr<Spreadsheet> t_spreasheet) : m_spreadsheet(t_spreasheet) {
}

void SpreadsheetRenderer::render() {
    if (ImGui::BeginChild("TableChild", ImVec2(0, 0), true, CHILD_FLAGS)) {
        renderTable();
        ImGui::EndChild();
    }
}

void SpreadsheetRenderer::renderTable() {
    auto spreadsheet = m_spreadsheet.lock();
    if (!spreadsheet)
        return;

    auto mesh = spreadsheet->mesh();
    if (!mesh)
        return;

    switch (spreadsheet->kind()) {
    case Spreadsheet::AttributeKind::POINTS:
        drawTable(mesh->pointAttribs);
        break;
    case Spreadsheet::AttributeKind::PRIMITIVES:
        drawTable(mesh->primAttribs);
        break;
    }
}

void SpreadsheetRenderer::drawTable(const AttributeSet& t_attributes) {
    size_t columns = 1; // init to 1 to add "id" column
    const size_t numAttributes = t_attributes.count();
    for (size_t attrIdx = 0; attrIdx < numAttributes; ++attrIdx) {
        columns += t_attributes.get(attrIdx)->attrSize();
    }

    if (ImGui::BeginTable("SpreadsheetTable", columns, TABLE_FLAGS)) {
        // ----- HEADERS ----- //
        ImGui::TableSetupColumn("id");
        for (size_t attrIdx = 0; attrIdx < numAttributes; ++attrIdx) {
            const Attribute* attribute = t_attributes.get(attrIdx);
            const size_t attrSize = attribute->attrSize();
            if (attrSize == 1) {
                ImGui::TableSetupColumn(attribute->name().c_str());
                continue;
            }

            for (size_t c = 0; c < attrSize; ++c) {
                const std::string columnName = attribute->name() + "[" + std::to_string(c) + "]";
                ImGui::TableSetupColumn(columnName.c_str());
            }
        }
        ImGui::TableHeadersRow();

        const size_t numElements = t_attributes.size();
        for (size_t i = 0; i < numElements; ++i) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text(std::to_string(i).c_str());
            for (size_t attrIdx = 0; attrIdx < numAttributes; ++attrIdx) {
                const Attribute* attribute = t_attributes.get(attrIdx);
                const size_t attrSize = attribute->attrSize();

                switch (attribute->type()) {
                case AttributeType::ATTR_TYPE_FLOAT: {
                    for (size_t c = 0; c < attrSize; ++c) {
                        ImGui::TableNextColumn();
                        ImGui::Text(std::to_string(attribute->component<float>(c)[i]).c_str());
                    }
                } break;
                }
            }
        }
        ImGui::EndTable();
    }
}

} // namespace euclide