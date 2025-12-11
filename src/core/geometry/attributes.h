#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "utils/allocator_utils.h"

namespace euclide {
struct AttributeBase {
    std::string name;
    int tupleSize;
    virtual ~AttributeBase() = default;
};

template <typename T>
struct Attribute : AttributeBase {
    std::vector<T, AlignedAllocator<T, 32>> data;
};

struct AttributeSet {
    std::vector<std::unique_ptr<AttributeBase>> attributes;
    std::unordered_map<std::string, int> index;
};

} // namespace euclide