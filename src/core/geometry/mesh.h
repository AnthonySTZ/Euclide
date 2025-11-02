#pragma once

#include "points.h"
#include "vertex.h"
#include "primitive.h"

namespace butter {

class Mesh {
    Mesh() = default;
    ~Mesh() = default;
    
private:
    Points m_points;
    std::vector<Vertex> m_vertices;
    std::vector<Primitive> m_primitives;
    
};

}