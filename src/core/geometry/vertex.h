#pragma once

#include <cstdlib>

namespace butter {

struct Vertex {

    size_t refPoint = 0; // Reference point index
    size_t primIndex = 0; // Which primitive this vertex belongs to

};

}