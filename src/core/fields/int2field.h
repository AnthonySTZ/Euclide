#pragma once

#include "node_field.h"

#include <array>

namespace butter {

class Int2Field: public NodeField<int2> {

public:
    Int2Field(int2 t_defaultValue) : NodeField<int2>(t_defaultValue) {}
    Int2Field(const int t_x, const int t_y) : NodeField<int2>(int2{t_x, t_y}) {}

    void setValue(const int t_x, const int t_y) {
        NodeField<int2>::setValue(int2{t_x, t_y});
    }

};

}