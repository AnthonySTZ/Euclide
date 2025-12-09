#include "copy_to_points.h"

#include "utils/timer.h"

#define USE_SIMD

#ifdef USE_SIMD
#include "utils/simd.h"
#endif

namespace euclide {

CopyToPoints::CopyToPoints() : Node(2, 1, "CopyToPoints") {
}

std::shared_ptr<Mesh> CopyToPoints::compute(const size_t t_index,
                                            const std::vector<std::shared_ptr<Mesh>>& t_inputs) const {
    if (t_inputs[0] == nullptr && t_inputs[1] == nullptr)
        return std::make_shared<Mesh>();
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>(*t_inputs[1]);
    if (t_inputs[1] == nullptr)
        return std::make_shared<Mesh>(*t_inputs[0]);

    auto output = std::make_shared<Mesh>(*t_inputs[0]);
    Timer timer{"CopyToPoints"};

    copyToPoints(*output, *t_inputs[1]);

    return output;
}

void CopyToPoints::copyToPoints(Mesh& t_mesh, const Mesh& t_points) {
    const auto points_to_copy = t_mesh.points;
    auto& new_points = t_mesh.points;
    const auto& points = t_points.points;
    const size_t numOfDuplicates = points.size();

    const size_t pointsSize = points_to_copy.size();
    new_points.clear();
    new_points.resize(pointsSize * numOfDuplicates);

    for (size_t i = 0; i < numOfDuplicates; ++i) {
        const float offsetX = points.posX[i];
        const float offsetY = points.posY[i];
        const float offsetZ = points.posZ[i];

        const size_t offsetIdx = i * pointsSize;
        size_t j = 0;

#ifdef USE_SIMD

        if (pointsSize >= 8) {
            __m256 __offsetX = _mm256_set1_ps(offsetX);
            __m256 __offsetY = _mm256_set1_ps(offsetY);
            __m256 __offsetZ = _mm256_set1_ps(offsetZ);
            for (; j + 8 <= pointsSize; j += 8) {
                const size_t pointIdx = offsetIdx + j;
                __m256 __posX = _mm256_load_ps(&points_to_copy.posX[j]);
                __m256 __posY = _mm256_load_ps(&points_to_copy.posY[j]);
                __m256 __posZ = _mm256_load_ps(&points_to_copy.posZ[j]);

                _mm256_storeu_ps(&new_points.posX[pointIdx], _mm256_add_ps(__posX, __offsetX));
                _mm256_storeu_ps(&new_points.posY[pointIdx], _mm256_add_ps(__posY, __offsetY));
                _mm256_storeu_ps(&new_points.posZ[pointIdx], _mm256_add_ps(__posZ, __offsetZ));

                _mm256_storeu_ps(&new_points.normalX[pointIdx], _mm256_load_ps(&points_to_copy.normalX[j]));
                _mm256_storeu_ps(&new_points.normalY[pointIdx], _mm256_load_ps(&points_to_copy.normalY[j]));
                _mm256_storeu_ps(&new_points.normalZ[pointIdx], _mm256_load_ps(&points_to_copy.normalZ[j]));

                _mm256_storeu_ps(&new_points.colorR[pointIdx], _mm256_load_ps(&points_to_copy.colorR[j]));
                _mm256_storeu_ps(&new_points.colorG[pointIdx], _mm256_load_ps(&points_to_copy.colorG[j]));
                _mm256_storeu_ps(&new_points.colorB[pointIdx], _mm256_load_ps(&points_to_copy.colorB[j]));
            }
        }

#endif

        for (; j < pointsSize; ++j) {
            const size_t pointIdx = offsetIdx + j;
            new_points.posX[pointIdx] = points_to_copy.posX[j] + offsetX;
            new_points.posY[pointIdx] = points_to_copy.posY[j] + offsetY;
            new_points.posZ[pointIdx] = points_to_copy.posZ[j] + offsetZ;

            new_points.normalX[pointIdx] = points_to_copy.normalX[j];
            new_points.normalY[pointIdx] = points_to_copy.normalY[j];
            new_points.normalZ[pointIdx] = points_to_copy.normalZ[j];

            new_points.colorR[pointIdx] = points_to_copy.colorR[j];
            new_points.colorG[pointIdx] = points_to_copy.colorG[j];
            new_points.colorB[pointIdx] = points_to_copy.colorB[j];
        }
    }

    auto& vertices = t_mesh.vertices;
    const size_t verticesSize = vertices.size();

    vertices.resize(verticesSize + verticesSize * numOfDuplicates);
    for (size_t i = 1; i <= numOfDuplicates; ++i) {
        const size_t vertOffset = verticesSize * i;
        const uint32_t pointOffset = pointsSize * i;
        for (size_t j = 0; j < verticesSize; ++j) {
            vertices[vertOffset + j] = vertices[j];
            vertices[vertOffset + j].refPoint += pointOffset;
        }
    }

    auto& primitives = t_mesh.primitives;
    const size_t primitivesSize = primitives.size();
    primitives.resize(primitivesSize + primitivesSize * numOfDuplicates);
    for (size_t i = 1; i <= numOfDuplicates; ++i) {
        const size_t primOffset = primitivesSize * i;
        const uint32_t verticesOffset = verticesSize * i;
        for (size_t j = 0; j < primitivesSize; ++j) {
            primitives[primOffset + j] = primitives[j];
            primitives[primOffset + j].verticesIndex += verticesOffset;
        }
    }
}

} // namespace euclide