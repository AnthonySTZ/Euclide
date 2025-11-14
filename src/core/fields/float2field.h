#pragma once

#include "node_field.h"

#include <array>

namespace butter {

class Float2Field: public NodeField<float2> {

public:
    Float2Field(float2 t_defaultValue) : NodeField<float2>(t_defaultValue) {}
    Float2Field(const float t_x, const float t_y) : NodeField<float2>(float2{t_x, t_y}) {}

    void setValue(const float t_x, const float t_y) {
        NodeField<float2>::setValue(float2{t_x, t_y});
    }

};

}