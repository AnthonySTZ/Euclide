#pragma once

#include <bits/stdc++.h>

namespace euclide {

float random() {
    return (float)(std::rand()) / (float)(RAND_MAX);
}

float random(float t_min, float t_max, float t_seed) {
    if (t_min > t_max)
        return random(t_max, t_min, t_seed);
    if (t_min == t_max)
        return t_min;

    float rd = (float)std::rand();
    rd = rd >= (t_max - t_min) ? 0 : rd;
    return t_min + rd + random();
}

} // namespace euclide