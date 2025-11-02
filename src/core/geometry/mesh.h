#pragma once

#include "points.h"
#include "vertex.h"
#include "primitive.h"

namespace butter {

struct Mesh {
    Points points;
    std::vector<Vertex> vertices;
    std::vector<Primitive> primitives;
};

}