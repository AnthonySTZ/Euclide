#include "smooth_normals.h"

#include <cmath>

namespace euclide {

SmoothNormals::SmoothNormals() : Node(1, 1, "SmoothNormals") {
}

std::shared_ptr<Mesh> SmoothNormals::compute(const size_t t_index,
                                             const std::vector<std::shared_ptr<Mesh>>& t_inputs) const {
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>();
    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    smoothNormals(*output);

    return output;
}

void SmoothNormals::smoothNormals(Mesh& t_mesh) {
    auto& pointAttribs = t_mesh.pointAttribs;
    if (pointAttribs.size() == 0)
        return;
    auto& prims = t_mesh.primitives;
    auto& vertices = t_mesh.vertices;

    auto normals = pointAttribs.findOrCreate<float, 3>("N");
    float* pointsNormalX = normals->component<float>(0);
    float* pointsNormalY = normals->component<float>(1);
    float* pointsNormalZ = normals->component<float>(2);

    auto positions = pointAttribs.findOrCreate<float, 3>("P");
    float* pointsPosX = positions->component<float>(0);
    float* pointsPosY = positions->component<float>(1);
    float* pointsPosZ = positions->component<float>(2);

    for (const auto& prim : prims) {
        if (prim.numVertices <= 2)
            continue;

        const uint32_t vertIdx = prim.verticesIndex;

        const uint32_t pointIdx_1 = vertices[vertIdx].refPoint;
        const uint32_t pointIdx_2 = vertices[vertIdx + 1].refPoint;
        const uint32_t pointIdx_3 = vertices[vertIdx + 2].refPoint;

        const float posX_1 = pointsPosX[pointIdx_1];
        const float posY_1 = pointsPosY[pointIdx_1];
        const float posZ_1 = pointsPosZ[pointIdx_1];

        const float dirX_1 = pointsPosX[pointIdx_2] - posX_1;
        const float dirY_1 = pointsPosY[pointIdx_2] - posY_1;
        const float dirZ_1 = pointsPosZ[pointIdx_2] - posZ_1;

        const float dirX_2 = pointsPosX[pointIdx_3] - posX_1;
        const float dirY_2 = pointsPosY[pointIdx_3] - posY_1;
        const float dirZ_2 = pointsPosZ[pointIdx_3] - posZ_1;

        const float primNormalX = (dirY_1 * dirZ_2) - (dirZ_1 * dirY_2);
        const float primNormalY = (dirZ_1 * dirX_2) - (dirX_1 * dirZ_2);
        const float primNormalZ = (dirX_1 * dirY_2) - (dirY_1 * dirX_2);

        for (size_t i = vertIdx; i < vertIdx + prim.numVertices; ++i) {
            const uint32_t pointIdx = vertices[i].refPoint;
            pointsNormalX[pointIdx] += primNormalX;
            pointsNormalY[pointIdx] += primNormalY;
            pointsNormalZ[pointIdx] += primNormalZ;
        }
    }

    for (size_t i = 0; i < pointAttribs.size(); ++i) {
        const float normalX = pointsNormalX[i];
        const float normalY = pointsNormalY[i];
        const float normalZ = pointsNormalZ[i];
        const float length = std::sqrt(normalX * normalX + normalY * normalY + normalZ * normalZ);
        if (length != 0) {
            const float factor = 1.0f / length;
            pointsNormalX[i] = normalX * factor;
            pointsNormalY[i] = normalY * factor;
            pointsNormalZ[i] = normalZ * factor;
        }
    }
}

} // namespace euclide