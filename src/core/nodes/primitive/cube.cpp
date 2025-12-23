#include "cube.h"

namespace euclide {

Cube::Cube() : Node(0, 1, "Cube") {
    auto positionField = std::make_shared<Float3Field>(0.0, 0.0, 0.0);
    positionField->setMetadata(NodeFieldMetadata{displayName : "Position", step : 0.02f});
    addField("position", positionField);

    auto sizeField = std::make_shared<Float3Field>(1.0, 1.0, 1.0);
    sizeField->setMetadata(NodeFieldMetadata{displayName : "Size", min : 0.0f, step : 0.02f});
    addField("size", sizeField);
}

std::shared_ptr<Mesh> Cube::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) {
    auto output = std::make_shared<Mesh>();

    float3 position = getField<Float3Field>("position")->getValue();
    float3 size = getField<Float3Field>("size")->getValue();

    const CubeSettings settings{position, size};

    createCube(*output, settings);

    return output;
}

void Cube::createCube(Mesh& t_mesh, const CubeSettings& t_settings) {
    const float3& position = t_settings.position;
    float3 size = t_settings.size;

    size[0] *= 0.5;
    size[1] *= 0.5;
    size[2] *= 0.5;

    t_mesh.pointAttribs.resize(8);

    auto positions = t_mesh.pointAttribs.findOrCreate<float, 3>("P");
    float* points_posX = positions->component<float>(0);
    float* points_posY = positions->component<float>(1);
    float* points_posZ = positions->component<float>(2);

    auto normals = t_mesh.pointAttribs.findOrCreate<float, 3>("N");
    float* points_normalX = normals->component<float>(0);
    float* points_normalY = normals->component<float>(1);
    float* points_normalZ = normals->component<float>(2);

    const float posX = size[0] + position[0];
    const float invPosX = -size[0] + position[0];
    const float posY = size[1] + position[1];
    const float invPosY = -size[1] + position[1];
    const float posZ = size[2] + position[2];
    const float invPosZ = -size[2] + position[2];

    constexpr float one_third = 1.0f / 3.0f;

    /* Top Face */
    points_posX[0] = posX;
    points_posX[1] = invPosX;
    points_posX[2] = invPosX;
    points_posX[3] = posX;
    points_posX[4] = posX;
    points_posX[5] = invPosX;
    points_posX[6] = invPosX;
    points_posX[7] = posX;
    points_normalX[0] = one_third;
    points_normalX[1] = -one_third;
    points_normalX[2] = -one_third;
    points_normalX[3] = one_third;
    points_normalX[4] = one_third;
    points_normalX[5] = -one_third;
    points_normalX[6] = -one_third;
    points_normalX[7] = one_third;

    points_posY[0] = posY;
    points_posY[1] = posY;
    points_posY[2] = posY;
    points_posY[3] = posY;
    points_posY[4] = invPosY;
    points_posY[5] = invPosY;
    points_posY[6] = invPosY;
    points_posY[7] = invPosY;
    points_normalY[0] = one_third;
    points_normalY[1] = one_third;
    points_normalY[2] = one_third;
    points_normalY[3] = one_third;
    points_normalY[4] = -one_third;
    points_normalY[5] = -one_third;
    points_normalY[6] = -one_third;
    points_normalY[7] = -one_third;

    points_posZ[0] = invPosZ;
    points_posZ[1] = invPosZ;
    points_posZ[2] = posZ;
    points_posZ[3] = posZ;
    points_posZ[4] = invPosZ;
    points_posZ[5] = invPosZ;
    points_posZ[6] = posZ;
    points_posZ[7] = posZ;
    points_normalZ[0] = -one_third;
    points_normalZ[1] = -one_third;
    points_normalZ[2] = one_third;
    points_normalZ[3] = one_third;
    points_normalZ[4] = -one_third;
    points_normalZ[5] = -one_third;
    points_normalZ[6] = one_third;
    points_normalZ[7] = one_third;

    /* Clockwise order */
    t_mesh.addPrimitive({0, 1, 2, 3}); // +Y
    t_mesh.addPrimitive({7, 6, 5, 4}); // -Y

    t_mesh.addPrimitive({4, 5, 1, 0}); // -Z
    t_mesh.addPrimitive({3, 2, 6, 7}); // +Z

    t_mesh.addPrimitive({0, 3, 7, 4}); // +X
    t_mesh.addPrimitive({1, 5, 6, 2}); // -X
}

} // namespace euclide