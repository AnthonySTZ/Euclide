#pragma once

#include <vector>
#include "utils/allocator_utils.h"

namespace butter {

struct Points {

    Points() = default;
    ~Points() = default;

    std::vector<float, AlignedAllocator<float, 32>> posX, posY, posZ; // use SoA instead of AoS

    size_t size() const noexcept { return posX.size(); }
    size_t addPoint(const float t_x, const float t_y, const float t_z) noexcept;

};


}