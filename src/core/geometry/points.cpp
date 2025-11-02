#include "points.h"

namespace butter {

/**
 * @brief Reserve a given number of point.
 * 
 * @param t_amount The number of points to reserve.
 * 
 * This function allocates memory for new points.
 */
void Points::reserve(const size_t t_amount)
{
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

/**
 * @brief Clear all points in `Points`
 */
void Points::clear() noexcept
{
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