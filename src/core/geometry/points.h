#pragma once

#include <vector>
#include "utils/allocator_utils.h"

namespace butter {

struct Points {

    Points() = default;
    ~Points() = default;

    std::vector<float, AlignedAllocator<float, 32>> posX, posY, posZ; // use SoA instead of AoS
    std::vector<float, AlignedAllocator<float, 32>> normalX, normalY, normalZ;
    std::vector<float, AlignedAllocator<float, 32>> colorR, colorG, colorB;

    void reserve(const size_t t_amount);
    void clear() noexcept;
    size_t size() const noexcept { return posX.size(); }
    size_t addPoint(const float t_x, const float t_y, const float t_z) noexcept;

};


}