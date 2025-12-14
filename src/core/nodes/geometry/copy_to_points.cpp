#include "copy_to_points.h"

#include "utils/timer.h"

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
    const auto pointsToCopy = t_mesh.pointAttribs;
    auto& newPoints = t_mesh.pointAttribs;
    const auto& points = t_points.pointAttribs;
    const size_t numOfDuplicates = points.size();
    if (numOfDuplicates == 0) {
        return;
    }

    const size_t pointsSize = pointsToCopy.size();
    if (pointsSize == 0) {
        return;
    }

    auto newPointsPositions = newPoints.findOrCreate<float, 3>("P");
    newPoints.resize(pointsSize * numOfDuplicates);

    const auto pointsPositions = points.find("P");
    const float* pointsPosX = pointsPositions->component<float>(0);
    const float* pointsPosY = pointsPositions->component<float>(1);
    const float* pointsPosZ = pointsPositions->component<float>(2);

    const auto pointsToCopyPositions = pointsToCopy.find("P");
    const float* pointsToCopyPosX = pointsToCopyPositions->component<float>(0);
    const float* pointsToCopyPosY = pointsToCopyPositions->component<float>(1);
    const float* pointsToCopyPosZ = pointsToCopyPositions->component<float>(2);

    float* newPointsPosX = newPointsPositions->component<float>(0);
    float* newPointsPosY = newPointsPositions->component<float>(1);
    float* newPointsPosZ = newPointsPositions->component<float>(2);

    for (size_t i = 0; i < numOfDuplicates; ++i) {
        const float offsetX = pointsPosX[i];
        const float offsetY = pointsPosY[i];
        const float offsetZ = pointsPosZ[i];

        const size_t offsetIdx = i * pointsSize;
        size_t j = 0;

#ifdef USE_SIMD

        if (pointsSize >= 8) {
            __m256 __offsetX = _mm256_set1_ps(offsetX);
            __m256 __offsetY = _mm256_set1_ps(offsetY);
            __m256 __offsetZ = _mm256_set1_ps(offsetZ);
            for (; j + 8 <= pointsSize; j += 8) {
                const size_t pointIdx = offsetIdx + j;
                __m256 __posX = _mm256_load_ps(&pointsToCopyPosX[j]);
                __m256 __posY = _mm256_load_ps(&pointsToCopyPosY[j]);
                __m256 __posZ = _mm256_load_ps(&pointsToCopyPosZ[j]);

                _mm256_storeu_ps(&newPointsPosX[pointIdx], _mm256_add_ps(__posX, __offsetX));
                _mm256_storeu_ps(&newPointsPosY[pointIdx], _mm256_add_ps(__posY, __offsetY));
                _mm256_storeu_ps(&newPointsPosZ[pointIdx], _mm256_add_ps(__posZ, __offsetZ));
            }
        }

#endif

        for (; j < pointsSize; ++j) {
            const size_t pointIdx = offsetIdx + j;
            newPointsPosX[pointIdx] = pointsToCopyPosX[j] + offsetX;
            newPointsPosY[pointIdx] = pointsToCopyPosY[j] + offsetY;
            newPointsPosZ[pointIdx] = pointsToCopyPosZ[j] + offsetZ;
        }
    }

    // Copy other attributes
    for (size_t attrIdx = 0; attrIdx < pointsToCopy.count(); ++attrIdx) {
        auto attributeToCopy = pointsToCopy.get(attrIdx);
        if (attributeToCopy->name() == "P")
            continue;
        auto newAttribute = newPoints.find(attributeToCopy->name());
        for (size_t i = 0; i < numOfDuplicates; ++i) {
            newAttribute->copyAt(*pointsToCopy.get(attrIdx), pointsSize * i, pointsSize);
        }
    }

    // ----- Copy vertices and prims ----- //
    // TODO: copy vertex attribs and prim attribs
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