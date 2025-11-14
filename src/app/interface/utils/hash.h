#pragma once

#include <cstdint>
#include <utility>
#include <bitset>

namespace butter {

struct EdgeHash {
    size_t operator()(const std::pair<uint32_t, uint32_t>& e) const noexcept {
        return std::hash<uint32_t>()(e.first) ^ (std::hash<uint32_t>()(e.second) << 1);
    }
};

}