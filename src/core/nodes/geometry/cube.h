#pragma once

#include "nodes/node.h"

#include "fields/float3field.h"

namespace butter {

class Cube: public Node {
    
public:
    Cube() : Node(2, 3, "Cube") {
        addField("position", std::make_shared<Float3Field>(0.0, 0.0, 0.0));
        addField("size", std::make_shared<Float3Field>(1.0, 1.0, 1.0));
    }

private:
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) override;

};
        
    
}