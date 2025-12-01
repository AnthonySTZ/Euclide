#pragma once

#include <vector>
#include <cstdint>
#include "utils/allocator_utils.h"

namespace euclide {

/// @brief Stores mesh points in a Structure-of-Arrays (SoA) format for SIMD-friendly access.
struct Points {
    Points() = default;
    ~Points() = default;

    std::vector<float, AlignedAllocator<float, 32>> posX, posY, posZ;          ///< Point positions.
    std::vector<float, AlignedAllocator<float, 32>> normalX, normalY, normalZ; ///< Point normals.
    std::vector<float, AlignedAllocator<float, 32>> colorR, colorG, colorB;    ///< Point colors.

    /// @brief Reserve memory for the specified number of points.
    /// @param t_amount Number of points to reserve space for.
    void reserve(const size_t t_amount);

    /// @brief Resize all internal arrays to hold the given number of points.
    /// @param t_amount New size of the points arrays.
    void resize(const size_t t_amount);

    /// @brief Clear all point data.
    void clear() noexcept;

    /// @brief Get the current number of points.
    /// @return Number of points.
    size_t size() const noexcept { return posX.size(); }

    /// @brief Add a new point to the container with default normal and color.
    /// @param t_x X-coordinate of the point.
    /// @param t_y Y-coordinate of the point.
    /// @param t_z Z-coordinate of the point.
    /// @return Index of the newly added point.
    uint32_t addPoint(const float t_x, const float t_y, const float t_z) noexcept;
};

} // namespace euclide
