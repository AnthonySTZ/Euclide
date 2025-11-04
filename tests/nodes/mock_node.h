#pragma once

#include "nodes/node.h"
#include "fields/node_field.h"

namespace butter {
    
class CreatePointNode: public Node {

public:
    CreatePointNode() : Node(0, 1, "Point") {
        m_fields.emplace("posX", std::make_shared<NodeField<float>>(5.0));
    };

private:
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) override {
        auto output = std::make_shared<Mesh>();
        output->points.addPoint(1.0, 2.0, 3.0); // predictable creation
        return output;
    }
};

class TestNode: public Node {
    
public:
    TestNode() : Node(1, 2, "Test") {};

private:
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) override {
        if (t_inputs.empty() || t_inputs[0] == nullptr) return nullptr;
        auto output = std::make_shared<Mesh>(*t_inputs[0]);

        if (t_index == 0) {
            output->points.addPoint(1.0, 0.0, 2.0); // predictable modification
        } else if (t_index == 1){
            output->points.addPoint(-8.0, 10.0, 20.0); // predictable modification
        }

        return output;
    }
};

}