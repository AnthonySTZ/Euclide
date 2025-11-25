#include "cube.h"

namespace butter {

Cube::Cube()
    : Node(0, 1, "Cube") 
{
    auto positionField = std::make_shared<Float3Field>(0.0, 0.0, 0.0);
    positionField->setMetadata(NodeFieldMetadata{
        displayName: "Position",
        step: 0.02f
    });
    addField("position", positionField);
    
    auto sizeField = std::make_shared<Float3Field>(1.0, 1.0, 1.0);
    sizeField->setMetadata(NodeFieldMetadata{
        displayName: "Size",
        min: 0.0f,
        step: 0.02f
    });
    addField("size", sizeField);
}

std::shared_ptr<Mesh> Cube::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>> &t_inputs) const
{
    auto output = std::make_shared<Mesh>();

    float3 position = getField<Float3Field>("position")->getValue();
    float3 size = getField<Float3Field>("size")->getValue();

    const CubeSettings settings{
        position,
        size
    };

    createCube(*output, settings);

    return output;
}

void Cube::createCube(Mesh &t_mesh, const CubeSettings &t_settings)
{
    const float3& position = t_settings.position;
    float3 size = t_settings.size;

    size[0] *= 0.5;
    size[1] *= 0.5;
    size[2] *= 0.5;

    /* Top Face */
    uint32_t p0 = t_mesh.addPoint(size[0] + position[0], size[1] + position[1], -size[2] + position[2]);
    uint32_t p1 = t_mesh.addPoint(-size[0] + position[0], size[1] + position[1], -size[2] + position[2]);
    uint32_t p2 = t_mesh.addPoint(-size[0] + position[0], size[1] + position[1], size[2] + position[2]);
    uint32_t p3 = t_mesh.addPoint(size[0] + position[0], size[1] + position[1], size[2] + position[2]);
    
    /* Bottom Face */
    uint32_t p4 = t_mesh.addPoint(size[0] + position[0], -size[1] + position[1], -size[2] + position[2]);
    uint32_t p5 = t_mesh.addPoint(-size[0] + position[0], -size[1] + position[1], -size[2] + position[2]);
    uint32_t p6 = t_mesh.addPoint(-size[0] + position[0], -size[1] + position[1], size[2] + position[2]);
    uint32_t p7 = t_mesh.addPoint(size[0] + position[0], -size[1] + position[1], size[2] + position[2]);

    /* Clockwise order */
    t_mesh.addPrimitive({p0, p1, p2, p3}); // +Y
    t_mesh.addPrimitive({p7, p6, p5, p4}); // -Y

    t_mesh.addPrimitive({p4, p5, p1, p0}); // -Z
    t_mesh.addPrimitive({p3, p2, p6, p7}); // +Z

    t_mesh.addPrimitive({p0, p3, p7, p4}); // +X
    t_mesh.addPrimitive({p1, p5, p6, p2}); // -X
}

}