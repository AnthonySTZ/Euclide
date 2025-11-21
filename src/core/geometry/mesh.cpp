#include "mesh.h"

#include "utils/timer.h"

#ifdef USE_SIMD
#include "utils/simd.h"
#endif

namespace butter {

float3 Mesh::center() const
{
    float3 sumPos{0.0, 0.0, 0.0};
    const size_t numPoints = points.size();
    if (numPoints == 0) return sumPos;
    
    size_t i = 0;

    #ifdef USE_SIMD
        if (numPoints >= 8) {
            __m256 __sumX = _mm256_setzero_ps();
            __m256 __sumY = _mm256_setzero_ps();
            __m256 __sumZ = _mm256_setzero_ps();
            
            for (; i + 8 <= numPoints; i += 8){
                __m256 __posX = _mm256_load_ps(&points.posX[i]);
                __m256 __posY = _mm256_load_ps(&points.posY[i]);
                __m256 __posZ = _mm256_load_ps(&points.posZ[i]);
                
                __sumX = _mm256_add_ps(__sumX, __posX);
                __sumY = _mm256_add_ps(__sumY, __posY);
                __sumZ = _mm256_add_ps(__sumZ, __posZ);
            }
            
            sumPos[0] = _mm256_hsum_ps(__sumX);
            sumPos[1] = _mm256_hsum_ps(__sumY);
            sumPos[2] = _mm256_hsum_ps(__sumZ);
        }
    #endif

    for (; i < numPoints; ++i) {
        sumPos[0] += points.posX[i];
        sumPos[1] += points.posY[i];
        sumPos[2] += points.posZ[i];
    }

    const float divisor = static_cast<float>(numPoints);

    sumPos[0] /= divisor;
    sumPos[1] /= divisor;
    sumPos[2] /= divisor;

    return sumPos;
}

}