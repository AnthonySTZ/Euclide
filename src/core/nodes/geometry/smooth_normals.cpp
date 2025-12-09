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
    auto& points = t_mesh.points;
    auto& prims = t_mesh.primitives;
    auto& vertices = t_mesh.vertices;
    std::fill(points.normalX.begin(), points.normalX.end(), 0.0);
    std::fill(points.normalY.begin(), points.normalY.end(), 0.0);
    std::fill(points.normalZ.begin(), points.normalZ.end(), 0.0);

    for (const auto& prim : prims) {
        if (prim.numVertices <= 2)
            continue;

        const uint32_t vertIdx = prim.verticesIndex;

        const uint32_t pointIdx_1 = vertices[vertIdx].refPoint;
        const uint32_t pointIdx_2 = vertices[vertIdx + 1].refPoint;
        const uint32_t pointIdx_3 = vertices[vertIdx + 2].refPoint;

        const float posX_1 = points.posX[pointIdx_1];
        const float posY_1 = points.posY[pointIdx_1];
        const float posZ_1 = points.posZ[pointIdx_1];

        const float dirX_1 = points.posX[pointIdx_2] - posX_1;
        const float dirY_1 = points.posY[pointIdx_2] - posY_1;
        const float dirZ_1 = points.posZ[pointIdx_2] - posZ_1;

        const float dirX_2 = points.posX[pointIdx_3] - posX_1;
        const float dirY_2 = points.posY[pointIdx_3] - posY_1;
        const float dirZ_2 = points.posZ[pointIdx_3] - posZ_1;

        const float primNormalX = (dirY_1 * dirZ_2) - (dirZ_1 * dirY_2);
        const float primNormalY = (dirZ_1 * dirX_2) - (dirX_1 * dirZ_2);
        const float primNormalZ = (dirX_1 * dirY_2) - (dirY_1 * dirX_2);

        for (size_t i = vertIdx; i < vertIdx + prim.numVertices; ++i) {
            const uint32_t pointIdx = vertices[i].refPoint;
            points.normalX[pointIdx] += primNormalX;
            points.normalY[pointIdx] += primNormalY;
            points.normalZ[pointIdx] += primNormalZ;
        }
    }

    for (size_t i = 0; i < points.size(); ++i) {
        const float normalX = points.normalX[i];
        const float normalY = points.normalY[i];
        const float normalZ = points.normalZ[i];
        const float length = std::sqrt(normalX * normalX + normalY * normalY + normalZ * normalZ);
        if (length != 0) {
            const float factor = 1.0f / length;
            points.normalX[i] = normalX * factor;
            points.normalY[i] = normalY * factor;
            points.normalZ[i] = normalZ * factor;
        }
    }
}

} // namespace euclide