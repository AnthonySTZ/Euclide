#pragma once

#include "nodes/node.h"

#include "fields/float3field.h"
#include "fields/float2field.h"

namespace butter {

struct CylinderSettings {
    float3 position = {0.0, 0.0, 0.0};
    float2 radius = {1.0, 1.0};
    float height = 1.0;
    int divisions = 8;
    bool capped = true; 
};

class Cylinder: public Node {
    
public:
    Cylinder();

    static void createCylinder(Mesh& t_mesh, const CylinderSettings& t_settings);

private:
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) override;

};
        
    
}