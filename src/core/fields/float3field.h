#pragma once

#include "node_field.h"

namespace butter {

/// @brief Node field representing a 3-component floating-point vector (float3).
///        Inherits from NodeField<float3>.
class Float3Field : public NodeField<float3> {
  public:
    /// @brief Construct a Float3Field with a default float3 value.
    /// @param t_defaultValue The initial value for the field.
    Float3Field(float3 t_defaultValue) : NodeField<float3>(t_defaultValue) {}

    /// @brief Construct a Float3Field with individual x, y, and z components.
    /// @param t_x Initial x component.
    /// @param t_y Initial y component.
    /// @param t_z Initial z component.
    Float3Field(const float t_x, const float t_y, const float t_z) : NodeField<float3>(float3{t_x, t_y, t_z}) {}

    /// @brief Set the value of the field using individual x, y, and z components.
    /// @param t_x New x component.
    /// @param t_y New y component.
    /// @param t_z New z component.
    inline void setValue(const float t_x, const float t_y, const float t_z) {
        NodeField<float3>::setValue(float3{t_x, t_y, t_z});
    }
};

} // namespace butter
