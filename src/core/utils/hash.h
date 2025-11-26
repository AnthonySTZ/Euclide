#include <utility>
#include <cstdint>

struct PairHash
{
    size_t operator()(const std::pair<uint32_t, uint32_t>& p) const noexcept
    {
        return (static_cast<size_t>(p.first) << 32) ^ p.second;
    }
};
