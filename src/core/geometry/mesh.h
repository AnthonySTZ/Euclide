#pragma once

#include "points.h"

namespace butter {

class Mesh {
    Mesh() = default;
    ~Mesh() = default;
    
private:
    Points m_points;
    
};

}