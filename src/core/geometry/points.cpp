#include "points.h"

namespace butter {

/**
 * @brief Add a point to the Points struct.
 *
 * @param t_x X coordinate of the point.
 * @param t_y Y coordinate of the point.
 * @param t_z Z coordinate of the point.
 * @return Index of the newly added point.
 *
 * This function appends the given coordinates to the internal SoA
 * storage (`posX`, `posY`, `posZ`) and returns the index of the new point.
*/
size_t Points::addPoint(const float t_x, const float t_y, const float t_z) noexcept
{
    posX.push_back(t_x);
    posY.push_back(t_y);
    posZ.push_back(t_z);

    normalX.push_back(0.0f);
    normalY.push_back(1.0f);
    normalZ.push_back(0.0f);

    colorR.push_back(1.0f);
    colorG.push_back(1.0f);
    colorB.push_back(1.0f);

    return posX.size() - 1;
}

}