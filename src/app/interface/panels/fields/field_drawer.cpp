#include "field_drawer.h"

namespace butter {
    
void FieldDrawer::drawIntField(const std::string& t_name, NodeField<int> &t_field)
{
    int value = t_field.getValue();
    const NodeFieldMetadata& meta = t_field.metadata();

    int min = meta.min.value_or(-INT_MAX);
    int max = meta.max.value_or(INT_MAX);
    int step = meta.step.value_or(0.05);
        
    const std::string sliderId = std::string("##param_") + t_name;
    if (ImGui::DragInt(sliderId.c_str(), &value, step, min, max)) {
        t_field.setValue(value);
    }
}

void FieldDrawer::drawInt2Field(const std::string& t_name, NodeField<int2> &t_field)
{
    int2 value = t_field.getValue();
    const NodeFieldMetadata& meta = t_field.metadata();

    int min = meta.min.value_or(-INT_MAX);
    int max = meta.max.value_or(INT_MAX);
    int step = meta.step.value_or(0.05);
        
    const std::string sliderId = std::string("##param_") + t_name;
    if (ImGui::DragInt2(sliderId.c_str(), value.data(), step, min, max)) {
        t_field.setValue(value);
    }
}

void FieldDrawer::drawFloatField(const std::string& t_name, NodeField<float> &t_field)
{
    float value = t_field.getValue();
    const NodeFieldMetadata& meta = t_field.metadata();

    float min = meta.min.value_or(-FLT_MAX);
    float max = meta.max.value_or(FLT_MAX);
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

    float min = meta.min.value_or(-FLT_MAX);
    float max = meta.max.value_or(FLT_MAX);
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

    float min = meta.min.value_or(-FLT_MAX);
    float max = meta.max.value_or(FLT_MAX);
    float step = meta.step.value_or(0.05);
        
    const std::string sliderId = std::string("##param_") + t_name;
    if (ImGui::DragFloat3(sliderId.c_str(), values.data(), step, min, max)) {
        t_field.setValue(values);
    }
}

}