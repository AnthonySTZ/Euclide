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

    auto& points = t_mesh.points;
    points.resize(8);

    const float posX = size[0] + position[0];
    const float invPosX = -size[0] + position[0];
    const float posY = size[1] + position[1];
    const float invPosY = -size[1] + position[1];
    const float posZ = size[2] + position[2];
    const float invPosZ = -size[2] + position[2];

    constexpr float one_third = 1.0f / 3.0f;

    /* Top Face */
    points.posX[0] = posX;
    points.posX[1] = invPosX;
    points.posX[2] = invPosX;
    points.posX[3] = posX;
    points.posX[4] = posX;
    points.posX[5] = invPosX;
    points.posX[6] = invPosX;
    points.posX[7] = posX;
    points.normalX[0] = one_third;
    points.normalX[1] = -one_third;
    points.normalX[2] = -one_third;
    points.normalX[3] = one_third;
    points.normalX[4] = one_third;
    points.normalX[5] = -one_third;
    points.normalX[6] = -one_third;
    points.normalX[7] = one_third;

    points.posY[0] = posY;
    points.posY[1] = posY;
    points.posY[2] = posY;
    points.posY[3] = posY;
    points.posY[4] = invPosY;
    points.posY[5] = invPosY;
    points.posY[6] = invPosY;
    points.posY[7] = invPosY;
    points.normalY[0] = one_third;
    points.normalY[1] = one_third;
    points.normalY[2] = one_third;
    points.normalY[3] = one_third;
    points.normalY[4] = -one_third;
    points.normalY[5] = -one_third;
    points.normalY[6] = -one_third;
    points.normalY[7] = -one_third;

    points.posZ[0] = invPosZ;
    points.posZ[1] = invPosZ;
    points.posZ[2] = posZ;
    points.posZ[3] = posZ;
    points.posZ[4] = invPosZ;
    points.posZ[5] = invPosZ;
    points.posZ[6] = posZ;
    points.posZ[7] = posZ;
    points.normalZ[0] = -one_third;
    points.normalZ[1] = -one_third;
    points.normalZ[2] = one_third;
    points.normalZ[3] = one_third;
    points.normalZ[4] = -one_third;
    points.normalZ[5] = -one_third;
    points.normalZ[6] = one_third;
    points.normalZ[7] = one_third;

    std::fill(std::begin(points.colorR), std::end(points.colorR), 1.0);
    std::fill(std::begin(points.colorG), std::end(points.colorG), 1.0);
    std::fill(std::begin(points.colorB), std::end(points.colorB), 1.0);

    /* Clockwise order */
    t_mesh.addPrimitive({0, 1, 2, 3}); // +Y
    t_mesh.addPrimitive({7, 6, 5, 4}); // -Y

    t_mesh.addPrimitive({4, 5, 1, 0}); // -Z
    t_mesh.addPrimitive({3, 2, 6, 7}); // +Z

    t_mesh.addPrimitive({0, 3, 7, 4}); // +X
    t_mesh.addPrimitive({1, 5, 6, 2}); // -X
}

}