#pragma once

#include "node_field.h"

#include <array>

namespace butter {

class Float3Field: public NodeField<vec3> {

public:
    Float3Field(vec3 t_defaultValue) : NodeField<vec3>(t_defaultValue) {}
    Float3Field(const float t_x, const float t_y, const float t_z) : NodeField<vec3>(vec3{t_x, t_y, t_z}) {}

    void setValue(const float t_x, const float t_y, const float t_z) {
        NodeField<vec3>::setValue(vec3{t_x, t_y, t_z});
    }

};

}