#pragma once

#include "nodes/node.h"

#include "fields/float3field.h"

namespace butter {

struct CubeSettings {
    float3 position;
    float3 size;
};

class Cube: public Node {
    
public:
    Cube();

    static void createCube(Mesh& t_mesh, const CubeSettings& t_settings);

private:
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) override;

};
        
    
}