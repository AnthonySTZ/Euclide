#include "points.h"

namespace euclide {

void Points::reserve(const size_t t_amount) {
    posX.reserve(t_amount);
    posY.reserve(t_amount);
    posZ.reserve(t_amount);
}

void Points::resize(const size_t t_amount) {
    posX.resize(t_amount);
    posY.resize(t_amount);
    posZ.resize(t_amount);
}

void Points::clear() noexcept {
    posX.clear();
    posY.clear();
    posZ.clear();
}

uint32_t Points::addPoint(const float t_x, const float t_y, const float t_z) noexcept {
    posX.push_back(t_x);
    posY.push_back(t_y);
    posZ.push_back(t_z);

    return static_cast<uint32_t>(posX.size() - 1);
}

} // namespace euclide