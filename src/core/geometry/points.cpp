#include "points.h"

void butter::Points::addPoint(const float t_x, const float t_y, const float t_z) noexcept
{
    posX.push_back(t_x);
    posY.push_back(t_y);
    posZ.push_back(t_z);
}
