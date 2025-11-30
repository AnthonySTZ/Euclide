#pragma once

#include "node_field.h"

namespace butter {

/// @brief Node field representing a 2-component integer vector (int2).
///        Inherits from NodeField<int2>.
class Int2Field : public NodeField<int2> {
  public:
    /// @brief Construct an Int2Field with a default int2 value.
    /// @param t_defaultValue The initial value for the field.
    Int2Field(int2 t_defaultValue) : NodeField<int2>(t_defaultValue) {}

    /// @brief Construct an Int2Field with individual x and y components.
    /// @param t_x Initial x component.
    /// @param t_y Initial y component.
    Int2Field(const int t_x, const int t_y) : NodeField<int2>(int2{t_x, t_y}) {}

    /// @brief Set the value of the field using individual x and y components.
    /// @param t_x New x component.
    /// @param t_y New y component.
    inline void setValue(const int t_x, const int t_y) { NodeField<int2>::setValue(int2{t_x, t_y}); }
};

} // namespace butter
