#pragma once
#include <cstdint>

namespace euclide {

/// @brief Computes a simple combined hash from two 32-bit unsigned integers.
///
/// This function shifts the first integer by 32 bits to the left and then
/// XORs it with the second integer, producing a single `size_t` hash value.
/// Useful for creating a unique key from a pair of integers, such as IDs.
///
/// @param t_first The first 32-bit unsigned integer.
/// @param t_second The second 32-bit unsigned integer.
/// @return A combined `size_t` hash of the two integers.
inline size_t hash(const uint32_t t_first, const uint32_t t_second) {
    return (static_cast<size_t>(t_first) << 32) ^ t_second;
}

} // namespace euclide
