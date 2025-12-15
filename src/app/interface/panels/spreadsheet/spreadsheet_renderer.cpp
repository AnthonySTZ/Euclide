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
    size_t columns = 0;
    const size_t numAttributes = t_attributes.count();
    for (size_t attrIdx = 0; attrIdx < numAttributes; ++attrIdx) {
        columns += t_attributes.get(attrIdx)->attrSize();
    }

    if (ImGui::BeginTable("SpreadsheetTable", columns, TABLE_FLAGS)) {
        // ----- HEADERS ----- //
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

        ImGui::EndTable();
    }
}

} // namespace euclide