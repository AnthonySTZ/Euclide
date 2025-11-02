#pragma once

#include "mesh.h"

#include <memory>

namespace butter {

class Node {

public:
    virtual std::shared_ptr<Mesh> compute(const std::shared_ptr<Mesh>& t_input) = 0;
};

}