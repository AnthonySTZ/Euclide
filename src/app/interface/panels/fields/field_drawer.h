#pragma once

#include "fields/node_field.h"
#include "interface/utils/imgui_utils.h"

namespace butter {

class FieldDrawer : public NodeFieldVisitor {

public:
void visit(const std::string& t_name, NodeField<int>& t_field) override {
        const auto& meta = t_field.metadata();
        if (meta.is_combo) {
            drawComboField(t_name, t_field);
        } else {
            drawIntField(t_name, t_field);
        }
    }
    void visit(const std::string& t_name, NodeField<int2>& t_field) override {
        drawInt2Field(t_name, t_field);
    }

    void visit(const std::string& t_name, NodeField<float>& t_field) override {
        drawFloatField(t_name, t_field);
    }
    void visit(const std::string& t_name, NodeField<float2>& t_field) override {
        drawFloat2Field(t_name, t_field);
    }
    void visit(const std::string& t_name, NodeField<float3>& t_field) override {
        drawFloat3Field(t_name, t_field);
    }


private:
    void drawComboField(const std::string& t_name, NodeField<int>& t_field);
    void drawIntField(const std::string& t_name, NodeField<int>& t_field);
    void drawInt2Field(const std::string& t_name, NodeField<int2>& t_field);

    void drawFloatField(const std::string& t_name, NodeField<float>& t_field);
    void drawFloat2Field(const std::string& t_name, NodeField<float2>& t_field);
    void drawFloat3Field(const std::string& t_name, NodeField<float3>& t_field);
};  

}