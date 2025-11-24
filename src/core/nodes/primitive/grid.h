#pragma once

#include "nodes/node.h"

namespace butter {

enum GridOrientation{
    ZX = 0,
    XY = 1,
    YZ = 2
};

struct GridSettings {
    float3 position;
    float2 size;
    int2 divisions;
    int orientation = GridOrientation::ZX;
};

class Grid: public Node {
    
public:
    Grid();

    static void createGrid(std::shared_ptr<Mesh> t_mesh, const GridSettings& t_settings);

private:
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) override;
};
        
    
}