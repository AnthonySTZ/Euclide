#include "copy_to_points.h"

#include "utils/timer.h"

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

    const size_t pointsSize = points_to_copy.size();
    new_points.clear();
    new_points.resize(pointsSize * points.size());

    for (size_t i = 0; i < points.size(); ++i) {
        const float offsetX = points.posX[i];
        const float offsetY = points.posY[i];
        const float offsetZ = points.posZ[i];

        const size_t offsetIdx = i * pointsSize;
        for (size_t j = 0; j < pointsSize; ++j) {
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
}

} // namespace euclide