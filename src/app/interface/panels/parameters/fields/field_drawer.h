#pragma once

#include "fields/node_field.h"
#include "interface/utils/imgui_utils.h"

namespace euclide {

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
    void visit(const std::string& t_name, NodeField<bool>& t_field) override { drawBoolField(t_name, t_field); }

    /// @brief Draws an int field or combo box depending on `metadata.is_combo`.
    void visit(const std::string& t_name, NodeField<int>& t_field) override {
        const auto& meta = t_field.metadata();
        if (meta.is_combo) {
            drawComboField(t_name, t_field);
        } else {
            drawIntField(t_name, t_field);
        }
    }

    /// @brief Draws a 2-component integer field.
    void visit(const std::string& t_name, NodeField<int2>& t_field) override { drawInt2Field(t_name, t_field); }

    /// @brief Draws a single float field.
    void visit(const std::string& t_name, NodeField<float>& t_field) override { drawFloatField(t_name, t_field); }

    /// @brief Draws a 2-component float field.
    void visit(const std::string& t_name, NodeField<float2>& t_field) override { drawFloat2Field(t_name, t_field); }

    /// @brief Draws a 3-component float field.
    void visit(const std::string& t_name, NodeField<float3>& t_field) override { drawFloat3Field(t_name, t_field); }

    /// @brief Draws a 4-component float field.
    void visit(const std::string& t_name, NodeField<float4>& t_field) override { drawFloat4Field(t_name, t_field); }

    /// @brief Draws a string field.
    void visit(const std::string& t_name, NodeField<std::string>& t_field) override {
        drawStringField(t_name, t_field);
    }

  private:
    /// @brief Generates a unique ImGui ID for a field from its name.
    ///
    /// ImGui requires unique IDs for widgets, even if multiple widgets display the same label.
    /// This function prepends a standard prefix to the given field name to produce a unique ID.
    ///
    /// @param t_name The field name.
    /// @return A string suitable for use as a unique ImGui widget ID.
    inline static std::string getFieldIdFromName(const std::string& t_name) noexcept { return "##param_" + t_name; }

    /// @brief Draws a boolean field as a checkbox within the UI.
    ///
    /// Displays a checkbox associated with the given field. If the user toggles
    /// the checkbox, the underlying value stored in the field will be updated.
    ///
    /// @param t_name  Unique and human-readable label used to identify the checkbox.
    /// @param t_field Reference to the boolean NodeField that holds the editable value.
    void drawBoolField(const std::string& t_name, NodeField<bool>& t_field);

    /// @brief Draws a combo box (dropdown UI element) linked to an integer field.
    ///
    /// The integer value of `t_field` is interpreted as an index into a list of selectable
    /// string labels stored in `t_field.metadata().choices`. When the user changes the
    /// selection, the field's value is updated to the newly chosen index.
    ///
    /// @note This function assumes that `choices` exists in the metadata and that the
    ///       current field value represents a valid index. If these assumptions are not
    ///       guaranteed, validation should be added.
    ///
    /// @param t_name  UI identifier used to uniquely reference the combo box.
    /// @param t_field Integer NodeField used to store and retrieve the selected index.
    void drawComboField(const std::string& t_name, NodeField<int>& t_field);

    /// @brief Draws an integer input field with optional clamping and step control.
    ///
    /// Renders an ImGui `DragInt` widget bound to the value inside `t_field`.
    /// Minimum value, maximum value, and step size (drag sensitivity) are taken
    /// from the field's metadata, if provided. If not present, the value is left
    /// unbounded and/or uses a default step of `1`.
    ///
    /// When the user adjusts the control and the value changes, the updated
    /// integer is written back to the underlying field.
    ///
    /// @param t_name  Name/identifier used to generate a unique ImGui widget ID.
    /// @param t_field Integer field whose stored value will be displayed and edited.
    void drawIntField(const std::string& t_name, NodeField<int>& t_field);

    /// @brief Draws a 2-component integer field with optional range limits and step size.
    ///
    /// Renders an ImGui `DragInt2` widget for editing a pair of integers stored in
    /// `t_field`. The value is treated as a 2-element vector (`int2`), and both components
    /// share the same minimum, maximum, and drag step — defined via metadata if present.
    /// If no metadata is provided, values remain unbounded and use a default step of `1`.
    ///
    /// When the user drags either component to a new value, the updated `int2` vector is
    /// written back into the field.
    ///
    /// @param t_name  Identifier used to generate a unique ImGui widget ID.
    /// @param t_field A `NodeField<int2>` whose two integer components will be displayed and edited.
    void drawInt2Field(const std::string& t_name, NodeField<int2>& t_field);

    /// @brief Draws a single float field with adjustable value, range, and sensitivity.
    ///
    /// Displays an ImGui `DragFloat` control linked to the value stored in `t_field`.
    /// The editable float will use metadata-defined constraints when available:
    ///
    ///   - `min` and `max` apply value clamping
    ///
    ///   - `step` defines drag sensitivity (default: 0.05)
    ///
    /// If the user changes the value through the widget, the new value is applied back
    /// to the underlying field.
    ///
    /// @param t_name  Name of the parameter, used to build a unique ImGui control ID.
    /// @param t_field A `NodeField<float>` whose numeric value will be displayed and edited.
    void drawFloatField(const std::string& t_name, NodeField<float>& t_field);

    /// @brief Draws a 2-component float field using an ImGui DragFloat2 control.
    ///
    /// Displays and allows editing of a `float2` value stored in the given field.
    /// Range limits and drag sensitivity are read from metadata:
    ///
    ///   - min / max define the allowed value range (if provided)
    ///
    ///   - step controls how fast the value changes when dragged (default = 0.05)
    ///
    /// When the user modifies either component, the updated vector is written
    /// back into the NodeField.
    ///
    /// @param t_name  UI name used to build a unique ImGui ID.
    /// @param t_field Field storing the editable float2 value.
    void drawFloat2Field(const std::string& t_name, NodeField<float2>& t_field);

    /// @brief Draws a 3-component float field using an ImGui DragFloat3 control.
    ///
    /// Displays and allows editing of a `float3` value stored in the given field.
    /// Range limits and drag sensitivity are read from metadata:
    ///
    ///   - min / max define the allowed value range (if provided)
    ///
    ///   - step controls how fast the value changes when dragged (default = 0.05)
    ///
    /// When the user modifies either component, the updated vector is written
    /// back into the NodeField.
    ///
    /// @param t_name  UI name used to build a unique ImGui ID.
    /// @param t_field Field storing the editable float3 value.
    void drawFloat3Field(const std::string& t_name, NodeField<float3>& t_field);

    /// @brief Draws a 4-component float field using an ImGui DragFloat4 control.
    ///
    /// Displays and allows editing of a `float4` value stored in the given field.
    /// Range limits and drag sensitivity are read from metadata:
    ///
    ///   - min / max define the allowed value range (if provided)
    ///
    ///   - step controls how fast the value changes when dragged (default = 0.05)
    ///
    /// When the user modifies either component, the updated vector is written
    /// back into the NodeField.
    ///
    /// @param t_name  UI name used to build a unique ImGui ID.
    /// @param t_field Field storing the editable float4 value.
    void drawFloat4Field(const std::string& t_name, NodeField<float4>& t_field);

    void drawStringField(const std::string& t_name, NodeField<std::string>& t_field);

  private:
    static constexpr float MAX_FLOAT = 3'999'999'999.9f; ///< Max float supported by slider
    static constexpr int MAX_INT = 2'000'000'000;        ///< Max int supported by slider
};

} // namespace euclide