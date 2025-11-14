#pragma once

#include "node_field.h"

#include <array>

namespace butter {

class Float3Field: public NodeField<float3> {

public:
    Float3Field(float3 t_defaultValue) : NodeField<float3>(t_defaultValue) {}
    Float3Field(const float t_x, const float t_y, const float t_z) : NodeField<float3>(float3{t_x, t_y, t_z}) {}

    void setValue(const float t_x, const float t_y, const float t_z) {
        NodeField<float3>::setValue(float3{t_x, t_y, t_z});
    }

};

}