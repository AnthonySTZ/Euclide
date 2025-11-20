#pragma once

#include <string>
#include <algorithm>

inline std::string toLower(std::string t_string) {
    std::transform(t_string.begin(), t_string.end(), t_string.begin(), [](unsigned char c) { return std::tolower(c); });
    return t_string;
}