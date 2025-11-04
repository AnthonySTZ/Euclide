#pragma once

#include "Vector.h"
#include "node_field.h"

namespace butter {

class Float3Field: public NodeField<py::vec3> {

public:
    Float3Field(py::vec3 t_defaultValue) : NodeField<py::vec3>(t_defaultValue) {}
    Float3Field(const float t_x, const float t_y, const float t_z) : NodeField<py::vec3>(py::vec3{t_x, t_y, t_z}) {}

    void setValue(const float t_x, const float t_y, const float t_z) {
        NodeField<py::vec3>::setValue(py::vec3{t_x, t_y, t_z});
    }

};

}