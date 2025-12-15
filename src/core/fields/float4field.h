#pragma once

#include "node_field.h"

namespace euclide {

/// @brief Node field representing a 4-component floating-point vector (float4).
///        Inherits from NodeField<float4>.
class Float4Field : public NodeField<float4> {
  public:
    /// @brief Construct a Float4Field with a default float4 value.
    /// @param t_defaultValue The initial value for the field.
    Float4Field(float4 t_defaultValue) : NodeField<float4>(t_defaultValue) {}

    /// @brief Construct a Float4Field with individual x, y, and z components.
    /// @param t_x Initial x component.
    /// @param t_y Initial y component.
    /// @param t_z Initial z component.
    Float4Field(const float t_x, const float t_y, const float t_z, const float t_w)
        : NodeField<float4>(float4{t_x, t_y, t_z, t_w}) {}

    /// @brief Set the value of the field using individual x, y, and z components.
    /// @param t_x New x component.
    /// @param t_y New y component.
    /// @param t_z New z component.
    inline void setValue(const float t_x, const float t_y, const float t_z, const float t_w) {
        NodeField<float4>::setValue(float4{t_x, t_y, t_z, t_w});
    }
};

} // namespace euclide
