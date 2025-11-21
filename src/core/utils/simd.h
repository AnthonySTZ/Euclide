#pragma once

#include <immintrin.h>

namespace butter {

inline float _mm256_hsum_ps(const __m256& __A) {
    __m128 lower = _mm256_castps256_ps128(__A); // __A = 8 floats to add -> split into two 128bits (4 floats), lower and higher
    __m128 higher = _mm256_extractf128_ps(__A, 1);
    __m128 sum_128 = _mm_add_ps(lower, higher); // add them into 4 floats
    sum_128 = _mm_hadd_ps(sum_128, sum_128); // horizontal add: [a, b, c, d] -> [a + b, c + d, a + b, c + d]
    sum_128 = _mm_hadd_ps(sum_128, sum_128); // horizontal add again: [(a + b) + (c + d), (a + b) + (c + d), (a + b) + (c + d), (a + b) + (c + d)]
                                    // which is equal to [sum, sum, sum, sum]
    return _mm_cvtss_f32(sum_128);
}

}