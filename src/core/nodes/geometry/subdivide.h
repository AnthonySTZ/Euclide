#pragma once

#include "nodes/node.h"

namespace butter {

struct SubdivideSettings{
    int divisions;
};

class Subdivide: public Node {
    
public:
    Subdivide();

    static void subdivide(Mesh& t_mesh, const SubdivideSettings& t_settings);

private:
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) const override;
};
}