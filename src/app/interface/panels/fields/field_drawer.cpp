#include "field_drawer.h"

constexpr float MAX_FLOAT = 1e9f;
constexpr int MAX_INT = 4'000'000'000;

namespace butter {

/// @brief Draws a boolean field as a checkbox within the UI.
/// 
/// Displays a checkbox associated with the given field. If the user toggles
/// the checkbox, the underlying value stored in the field will be updated.
///
/// @param t_name  Unique and human-readable label used to identify the checkbox.
/// @param t_field Reference to the boolean NodeField that holds the editable value.
void FieldDrawer::drawBoolField(const std::string &t_name, NodeField<bool> &t_field)
{
    bool value = t_field.getValue();

    const std::string sliderId = getFieldIdFromName(t_name);
    if (ImGui::Checkbox(sliderId.c_str(), &value)) {
        t_field.setValue(value);
    }
}

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
void FieldDrawer::drawComboField(const std::string &t_name, NodeField<int> &t_field)
{
    int index = t_field.getValue();
    const NodeFieldMetadata& meta = t_field.metadata();

    // Get choices from metadata and clamp index safely
    const auto& choices = meta.choices.value_or({});
    if (choices.empty()) {
        return; // nothing to display
    }
    if (index < 0 || index >= choices.size()) {
        index = 0;
        t_field.setValue(index);
    }
        
    const std::string comboId = getFieldIdFromName(t_name);
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
void FieldDrawer::drawIntField(const std::string& t_name, NodeField<int> &t_field)
{
    int value = t_field.getValue();
    const NodeFieldMetadata& meta = t_field.metadata();

    int min = static_cast<int>(meta.min.value_or(-MAX_INT));
    int max = static_cast<int>(meta.max.value_or(MAX_INT));
    int step = static_cast<int>(meta.step.value_or(1));
        
    const std::string sliderId = getFieldIdFromName(t_name);
    if (ImGui::DragInt(sliderId.c_str(), &value, step, min, max)) {
        t_field.setValue(value);
    }
}

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
void FieldDrawer::drawInt2Field(const std::string& t_name, NodeField<int2> &t_field)
{
    int2 value = t_field.getValue();
    const NodeFieldMetadata& meta = t_field.metadata();

    int min = static_cast<int>(meta.min.value_or(-MAX_INT));
    int max = static_cast<int>(meta.max.value_or(MAX_INT));
    int step = static_cast<int>(meta.step.value_or(1));
        
    const std::string sliderId = getFieldIdFromName(t_name);
    if (ImGui::DragInt2(sliderId.c_str(), value.data(), step, min, max)) {
        t_field.setValue(value);
    }
}

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
void FieldDrawer::drawFloatField(const std::string& t_name, NodeField<float> &t_field)
{
    float value = t_field.getValue();
    const NodeFieldMetadata& meta = t_field.metadata();

    float min = meta.min.value_or(-MAX_FLOAT);
    float max = meta.max.value_or(MAX_FLOAT);
    float step = meta.step.value_or(0.05);
        
    const std::string sliderId = getFieldIdFromName(t_name);
    if (ImGui::DragFloat(sliderId.c_str(), &value, step, min, max)) {
        t_field.setValue(value);
    }
}

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
void FieldDrawer::drawFloat2Field(const std::string& t_name, NodeField<float2> &t_field)
{
    float2 values = t_field.getValue();
    const NodeFieldMetadata& meta = t_field.metadata();

    float min = meta.min.value_or(-MAX_FLOAT);
    float max = meta.max.value_or(MAX_FLOAT);
    float step = meta.step.value_or(0.05);
        
    const std::string sliderId = getFieldIdFromName(t_name);
    if (ImGui::DragFloat2(sliderId.c_str(), values.data(), step, min, max)) {
        t_field.setValue(values);
    }
}

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
void FieldDrawer::drawFloat3Field(const std::string& t_name, NodeField<float3> &t_field)
{
    float3 values = t_field.getValue();
    const NodeFieldMetadata& meta = t_field.metadata();

    float min = meta.min.value_or(-MAX_FLOAT);
    float max = meta.max.value_or(MAX_FLOAT);
    float step = meta.step.value_or(0.05);
        
    const std::string sliderId = getFieldIdFromName(t_name);
    if (ImGui::DragFloat3(sliderId.c_str(), values.data(), step, min, max)) {
        t_field.setValue(values);
    }
}

}