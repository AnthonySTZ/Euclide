#pragma once

#include "nodes/node.h"

#include "fields/float3field.h"

namespace butter {

struct TransformSettings {
    float3 translation;
    float3 scale;
    float3 rotation;
};

class Transform: public Node {
    
public:
    Transform();

    static void transform(Mesh& t_mesh, const TransformSettings& t_settings);

    static void scaleMesh(Mesh& t_mesh, const float3 &t_scale);
    static void rotateMesh(Mesh& t_mesh, const float3 &t_rotation);
    static void translateMesh(Mesh& t_mesh, const float3 &t_translation);
    
private:
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) override;
    
};
}