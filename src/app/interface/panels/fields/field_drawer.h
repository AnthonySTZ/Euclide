#pragma once

#include "fields/node_field.h"
#include "interface/utils/imgui_utils.h"

namespace butter {

class FieldDrawer : public NodeFieldVisitor {

public:
    void visit(const std::string& t_name, NodeField<int>& t_field) override {
    }
    void visit(const std::string& t_name, NodeField<float>& t_field) override {
    }
    void visit(const std::string& t_name, NodeField<float3>& t_field) override {
        ImGui::Text(t_name.c_str());
        drawFloat3Field(t_name, t_field);
    }


private:
    void drawFloat3Field(const std::string& t_name, NodeField<float3>& t_field);
};  

}