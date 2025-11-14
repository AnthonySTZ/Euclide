#include "field_drawer.h"

namespace butter {
    
void FieldDrawer::drawFloat3Field(const std::string& t_name, NodeField<float3> &t_field)
{
    float3 values = t_field.getValue();
    
    const std::string sliderId = std::string("##slider_") + t_name;
    if (ImGui::SliderFloat3(sliderId.c_str(), values.data(), 0, 10)) {
        t_field.setValue(values);
    }
}

}