#pragma once

#include <cmath>

namespace euclide {

/// @brief Converts an angle from degrees to radians.
///
/// @param t_degrees Angle in degrees.
/// @return Angle in radians.
inline double radians(const double t_degrees) noexcept {
    return t_degrees * M_PI / 180.0;
}

/// @brief Converts an angle from radians to degrees.
///
/// @param t_radians Angle in radians.
/// @return Angle in degrees.
inline double degrees(const double t_radians) noexcept {
    return t_radians * 180.0 * M_1_PI;
}

} // namespace euclide
