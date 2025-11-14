#pragma once

#include "nodes/node.h"

#include "fields/float3field.h"

namespace butter {

class Cube: public Node {
    
public:
    Cube() : Node(0, 1, "Cube") {
        auto positionField = std::make_shared<Float3Field>(0.0, 0.0, 0.0);
        positionField->setMetadata(NodeFieldMetadata{
            displayName: "Position",
            min: -10.0f,
            max: 10.0f,
            step: 0.02f
        });
        addField("position", positionField);
        
        auto sizeField = std::make_shared<Float3Field>(1.0, 1.0, 1.0);
        sizeField->setMetadata(NodeFieldMetadata{
            displayName: "Size",
            min: 0.0f,
            max: 10.0f,
            step: 0.02f
        });
        addField("size", sizeField);
    }

private:
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) override;

};
        
    
}