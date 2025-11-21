#include "mesh.h"

#include "utils/timer.h"

namespace butter {

float3 Mesh::center() const
{
    if (points.size() == 0) return {0.0, 0.0, 0.0};

    Timer timer{"center"};

    float3 sumPos{0.0, 0.0, 0.0};
    size_t i = 0;
    for (; i < points.size(); ++i) {
        sumPos[0] += points.posX[i];
        sumPos[1] += points.posY[i];
        sumPos[2] += points.posZ[i];
    }

    const float divisor = static_cast<float>(points.size());

    sumPos[0] /= divisor;
    sumPos[1] /= divisor;
    sumPos[2] /= divisor;

    return sumPos;
}

}