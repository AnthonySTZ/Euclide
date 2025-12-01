#pragma once

#include "node_field.h"

namespace euclide {

/// @brief Node field representing a 2-component floating-point vector (float2).
///        Inherits from NodeField<float2>.
class Float2Field : public NodeField<float2> {
  public:
    /// @brief Construct a Float2Field with a default float2 value.
    /// @param t_defaultValue The initial value for the field.
    Float2Field(float2 t_defaultValue) : NodeField<float2>(t_defaultValue) {}

    /// @brief Construct a Float2Field with individual x and y components.
    /// @param t_x Initial x component.
    /// @param t_y Initial y component.
    Float2Field(const float t_x, const float t_y) : NodeField<float2>(float2{t_x, t_y}) {}

    /// @brief Set the value of the field using individual x and y components.
    /// @param t_x New x component.
    /// @param t_y New y component.
    inline void setValue(const float t_x, const float t_y) { NodeField<float2>::setValue(float2{t_x, t_y}); }
};

} // namespace euclide
