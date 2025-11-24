#include "field_drawer.h"

#define MAX_FLOAT 999999999.9f
#define MAX_INT 999999999

namespace butter {

void FieldDrawer::drawBoolField(const std::string &t_name, NodeField<bool> &t_field)
{
    bool value = t_field.getValue();

    const std::string sliderId = std::string("##param_") + t_name;
    if (ImGui::Checkbox(sliderId.c_str(), &value)) {
        t_field.setValue(value);
    }
}

void FieldDrawer::drawComboField(const std::string &t_name, NodeField<int> &t_field)
{
    int index = t_field.getValue();
    const NodeFieldMetadata& meta = t_field.metadata();

    const auto& choices = meta.choices.value_or({});
        
    const std::string comboId = std::string("##param_") + t_name;
    if (ImGui::BeginCombo(comboId.c_str(), choices[index].c_str())) {
        for (size_t i = 0; i < choices.size(); ++i) {
            bool is_selected = index == i;
            if (ImGui::Selectable(choices[i].c_str(), is_selected)){
                t_field.setValue(i);
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
    ImGui::EndCombo();
    }
}

void FieldDrawer::drawIntField(const std::string& t_name, NodeField<int> &t_field)
{
    int value = t_field.getValue();
    const NodeFieldMetadata& meta = t_field.metadata();

    int min = static_cast<int>(meta.min.value_or(-MAX_INT));
    int max = static_cast<int>(meta.max.value_or(MAX_INT));
    int step = static_cast<int>(meta.step.value_or(1));
        
    const std::string sliderId = std::string("##param_") + t_name;
    if (ImGui::DragInt(sliderId.c_str(), &value, step, min, max)) {
        t_field.setValue(value);
    }
}

void FieldDrawer::drawInt2Field(const std::string& t_name, NodeField<int2> &t_field)
{
    int2 value = t_field.getValue();
    const NodeFieldMetadata& meta = t_field.metadata();

    int min = static_cast<int>(meta.min.value_or(-MAX_INT));
    int max = static_cast<int>(meta.max.value_or(MAX_INT));
    int step = static_cast<int>(meta.step.value_or(1));
        
    const std::string sliderId = std::string("##param_") + t_name;
    if (ImGui::DragInt2(sliderId.c_str(), value.data(), step, min, max)) {
        t_field.setValue(value);
    }
}

void FieldDrawer::drawFloatField(const std::string& t_name, NodeField<float> &t_field)
{
    float value = t_field.getValue();
    const NodeFieldMetadata& meta = t_field.metadata();

    float min = meta.min.value_or(-MAX_FLOAT);
    float max = meta.max.value_or(MAX_FLOAT);
    float step = meta.step.value_or(0.05);
        
    const std::string sliderId = std::string("##param_") + t_name;
    if (ImGui::DragFloat(sliderId.c_str(), &value, step, min, max)) {
        t_field.setValue(value);
    }
}

void FieldDrawer::drawFloat2Field(const std::string& t_name, NodeField<float2> &t_field)
{
    float2 values = t_field.getValue();
    const NodeFieldMetadata& meta = t_field.metadata();

    float min = meta.min.value_or(-MAX_FLOAT);
    float max = meta.max.value_or(MAX_FLOAT);
    float step = meta.step.value_or(0.05);
        
    const std::string sliderId = std::string("##param_") + t_name;
    if (ImGui::DragFloat2(sliderId.c_str(), values.data(), step, min, max)) {
        t_field.setValue(values);
    }
}

void FieldDrawer::drawFloat3Field(const std::string& t_name, NodeField<float3> &t_field)
{
    float3 values = t_field.getValue();
    const NodeFieldMetadata& meta = t_field.metadata();

    float min = meta.min.value_or(-MAX_FLOAT);
    float max = meta.max.value_or(MAX_FLOAT);
    float step = meta.step.value_or(0.05);
        
    const std::string sliderId = std::string("##param_") + t_name;
    if (ImGui::DragFloat3(sliderId.c_str(), values.data(), step, min, max)) {
        t_field.setValue(values);
    }
}

}