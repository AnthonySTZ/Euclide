#pragma once

#include "nodes/node.h"

#include "fields/float3field.h"

namespace butter {

class Transform: public Node {
    
public:
    Transform();

private:
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) override;
    
    void scaleMesh(std::shared_ptr<Mesh> t_mesh, const float3 &t_scale);
    void rotateMesh(std::shared_ptr<Mesh> t_mesh, const float3 &t_rotation);
    void translateMesh(std::shared_ptr<Mesh> t_mesh, const float3 &t_translation);
};
}