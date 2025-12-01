#include "points.h"

namespace euclide {

void Points::reserve(const size_t t_amount) {
    posX.reserve(t_amount);
    posY.reserve(t_amount);
    posZ.reserve(t_amount);

    normalX.reserve(t_amount);
    normalY.reserve(t_amount);
    normalZ.reserve(t_amount);

    colorR.reserve(t_amount);
    colorG.reserve(t_amount);
    colorB.reserve(t_amount);
}

void Points::resize(const size_t t_amount) {
    posX.resize(t_amount);
    posY.resize(t_amount);
    posZ.resize(t_amount);

    normalX.resize(t_amount);
    normalY.resize(t_amount);
    normalZ.resize(t_amount);

    colorR.resize(t_amount);
    colorG.resize(t_amount);
    colorB.resize(t_amount);
}

void Points::clear() noexcept {
    posX.clear();
    posY.clear();
    posZ.clear();

    normalX.clear();
    normalY.clear();
    normalZ.clear();

    colorR.clear();
    colorG.clear();
    colorB.clear();
}

uint32_t Points::addPoint(const float t_x, const float t_y, const float t_z) noexcept {
    posX.push_back(t_x);
    posY.push_back(t_y);
    posZ.push_back(t_z);

    normalX.push_back(0.0f);
    normalY.push_back(1.0f);
    normalZ.push_back(0.0f);

    colorR.push_back(1.0f);
    colorG.push_back(1.0f);
    colorB.push_back(1.0f);

    return static_cast<uint32_t>(posX.size() - 1);
}

} // namespace euclide