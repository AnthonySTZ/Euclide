#pragma once

#include <cmath>

namespace butter {

inline double radians(const double t_degrees) noexcept
{
    return t_degrees * M_PI / 180.0;
}

inline double degrees(const double t_radians) noexcept
{
    return t_radians * 180.0 * M_1_PI;
}


}