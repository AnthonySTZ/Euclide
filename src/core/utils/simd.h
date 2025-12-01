#pragma once

#include <immintrin.h>

namespace euclide {

/// @brief Computes the horizontal sum of all 8 floats in a __m256 vector.
///
/// @param __A The __m256 vector containing 8 single-precision floats.
/// @return The sum of all 8 floats as a single float.
inline float _mm256_hsum_ps(const __m256& __A) {
    // Split __A into lower and higher 128-bit halves (4 floats each)
    __m128 lower = _mm256_castps256_ps128(__A);
    __m128 higher = _mm256_extractf128_ps(__A, 1);

    // Add the two halves element-wise to get 4 partial sums
    __m128 sum_128 = _mm_add_ps(lower, higher);

    // Horizontal add: [a, b, c, d] -> [a+b, c+d, a+b, c+d]
    sum_128 = _mm_hadd_ps(sum_128, sum_128);

    // Horizontal add again: [(a+b)+(c+d), ...] -> all elements equal to total sum
    sum_128 = _mm_hadd_ps(sum_128, sum_128);

    // Extract the first float which is the total sum
    return _mm_cvtss_f32(sum_128);
}

} // namespace euclide
