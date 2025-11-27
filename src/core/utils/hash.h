#include <utility>
#include <cstdint>

namespace butter {

inline size_t hash(const uint32_t t_first, const uint32_t t_second) {
    return (static_cast<size_t>(t_first) << 32) ^ t_second;
}

}