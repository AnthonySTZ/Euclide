#pragma once

#include "nodes/node.h"

namespace butter {

class Merge: public Node {
    
public:
    Merge();

    static void merge(Mesh& t_mesh, const Mesh& t_mesh_2);

private:
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) override;
};
}