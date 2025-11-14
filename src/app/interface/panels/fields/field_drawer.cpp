#include "field_drawer.h"

namespace butter {
    
void FieldDrawer::drawFloat3Field(const std::string& t_name, NodeField<float3> &t_field)
{
    float3 values = t_field.getValue();
    const NodeFieldMetadata& meta = t_field.metadata();

    float min = meta.min.value_or(-FLT_MAX);
    float max = meta.max.value_or(FLT_MAX);
    float step = meta.step.value_or(0.05);
        
    const std::string sliderId = std::string("##slider_") + t_name;
    if (ImGui::DragFloat3(sliderId.c_str(), values.data(), step, min, max)) {
        t_field.setValue(values);
    }
}

}