#pragma once

#include "fields/node_field.h"

#include <memory>

namespace butter {

class FieldDrawer : public NodeFieldVisitor {

public:
    void visit(const std::string& t_name, NodeField<int>& t_field) override {
    }
    void visit(const std::string& t_name, NodeField<float>& t_field) override {
    }
    void visit(const std::string& t_name, NodeField<float3>& t_field) override {
        ImGui::Text(t_name.c_str());
        float3 values = t_field.getValue();

        const std::string sliderId = std::string("##slider_") + t_name;
        if (ImGui::SliderFloat3(sliderId.c_str(), values.data(), 0, 10)) {
            t_field.setValue(values);
        }
    }
};  

}