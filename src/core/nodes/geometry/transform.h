#pragma once

#include "nodes/node.h"

#include "fields/float3field.h"

namespace butter {

class Transform: public Node {
    
public:
    Transform() : Node(1, 1, "Transform") {
        addField("translate", std::make_shared<Float3Field>(0.0, 0.0, 0.0));
        addField("size", std::make_shared<Float3Field>(1.0, 1.0, 1.0));
        addField("rotate", std::make_shared<Float3Field>(0.0, 0.0, 0.0));
    }

private:
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) override;

};
        
    
}