#pragma once

#include "fields/node_field.h"
#include "interface/utils/imgui_utils.h"

namespace butter {

/// @brief Utility class to render `NodeField` objects in an ImGui-based UI.
///
/// Implements NodeFieldVisitor and dispatches each field type to the
/// appropriate `draw*Field` method. Supports `bool`, `int`, `int2`, `float`, `float2`, `float3`.
///
/// Usage:
///
///     - `FieldDrawer drawer;`
///
///     - `field.accept(drawer);`
class FieldDrawer : public NodeFieldVisitor {

public:
    /// @brief Draws a boolean field using a checkbox.
    void visit(const std::string& t_name, NodeField<bool>& t_field) override {
        drawBoolField(t_name, t_field);        
    }
    /// @brief Draws an int field or combo box depending on metadata.
    void visit(const std::string& t_name, NodeField<int>& t_field) override {
        const auto& meta = t_field.metadata();
        if (meta.is_combo) {
            drawComboField(t_name, t_field);
        } else {
            drawIntField(t_name, t_field);
        }
    }
    /// @brief Draws a 2-component integer field.
    void visit(const std::string& t_name, NodeField<int2>& t_field) override {
        drawInt2Field(t_name, t_field);
    }
    /// @brief Draws a single float field.
    void visit(const std::string& t_name, NodeField<float>& t_field) override {
        drawFloatField(t_name, t_field);
    }
    /// @brief Draws a 2-component float field.
    void visit(const std::string& t_name, NodeField<float2>& t_field) override {
        drawFloat2Field(t_name, t_field);
    }
    /// @brief Draws a 3-component float field.
    void visit(const std::string& t_name, NodeField<float3>& t_field) override {
        drawFloat3Field(t_name, t_field);
    }


private:
    /// @brief Generates a unique ImGui ID for a field from its name.
    ///
    /// ImGui requires unique IDs for widgets, even if multiple widgets display the same label.
    /// This function prepends a standard prefix to the given field name to produce a unique ID.
    ///
    /// @param t_name The field name.
    /// @return A string suitable for use as a unique ImGui widget ID.
    inline static std::string getFieldIdFromName(const std::string& t_name) noexcept {
        return "##param_" + t_name; 
    }

    void drawBoolField(const std::string& t_name, NodeField<bool>& t_field);

    void drawComboField(const std::string& t_name, NodeField<int>& t_field);
    void drawIntField(const std::string& t_name, NodeField<int>& t_field);
    void drawInt2Field(const std::string& t_name, NodeField<int2>& t_field);

    void drawFloatField(const std::string& t_name, NodeField<float>& t_field);
    void drawFloat2Field(const std::string& t_name, NodeField<float2>& t_field);
    void drawFloat3Field(const std::string& t_name, NodeField<float3>& t_field);
};  

}