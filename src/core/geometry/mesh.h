#pragma once

#include "points.h"
#include "vertex.h"

namespace butter {

class Mesh {
    Mesh() = default;
    ~Mesh() = default;
    
private:
    Points m_points;
    std::vector<Vertex> m_vertices;
    
};

}