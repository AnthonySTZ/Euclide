#include "mesh.h"

#include "utils/timer.h"

#ifdef USE_SIMD
#include "utils/simd.h"
#endif

namespace butter {

float3 Mesh::center() const
{
    if (points.size() == 0) return {0.0, 0.0, 0.0};

    Timer timer{"center"};

    float3 sumPos{0.0, 0.0, 0.0};
    size_t i = 0;

    #ifdef USE_SIMD

        __m256 __sumX = _mm256_setzero_ps();
        __m256 __sumY = _mm256_setzero_ps();
        __m256 __sumZ = _mm256_setzero_ps();

        for (; i + 8 <= points.size(); i += 8){
            __m256 __posX = _mm256_load_ps(&points.posX[i]);
            __m256 __posY = _mm256_load_ps(&points.posY[i]);
            __m256 __posZ = _mm256_load_ps(&points.posZ[i]);

            __sumX = _mm256_add_ps(__sumX, __posX);
            __sumY = _mm256_add_ps(__sumY, __posY);
            __sumZ = _mm256_add_ps(__sumZ, __posZ);
        }

        _mm256_sum_store_ss(&sumPos[0], __sumX);
        _mm256_sum_store_ss(&sumPos[1], __sumY);
        _mm256_sum_store_ss(&sumPos[2], __sumZ);

    #endif

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