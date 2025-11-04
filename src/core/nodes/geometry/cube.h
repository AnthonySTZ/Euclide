#pragma once

#include "nodes/node.h"

namespace butter {

class Cube: public Node {
    
public:
    Cube() : Node(0, 1, "Cube") {}

private:
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) override;

};
        
    
}