#pragma once

#include <vector>

namespace butter {

struct Points {

    std::vector<float> posX, posY, posZ; // use SoA instead of AoS

    size_t size() const noexcept { return posX.size(); }
    void addPoint(const float t_x, const float t_y, const float t_z) noexcept;

};


}