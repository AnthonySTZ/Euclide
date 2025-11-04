#pragma once

#include <cstdint>

namespace butter {

struct Vertex {

    uint32_t refPoint = 0; // Reference point index
    uint32_t primIndex = 0; // Which primitive this vertex belongs to

};

}