#pragma once

#include <vector>
#include <cstdint>

namespace butter {

struct Primitive{

    uint32_t verticesIndex = UINT32_MAX; // index of first vertex in vertices
    uint32_t numVertices = UINT32_MAX;

};

}