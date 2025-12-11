#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "utils/allocator_utils.h"

namespace euclide {

enum class AttributeType {
    ATTR_TYPE_FLOAT,
};

struct AttributeBase {
    std::string name;
    int tupleSize;
    AttributeType type;

    virtual ~AttributeBase() = default;
    [[nodiscard]] inline AttributeType getType() const noexcept { return type; }

    [[nodiscard]] virtual void* data_ptr() = 0;
};

template <typename T>
struct Attribute : AttributeBase {
    std::vector<T, AlignedAllocator<T, 32>> data;
    [[nodiscard]] void* data_ptr() override { return data.data(); }
};

struct AttributeSet {
    size_t size = 0;
    std::vector<std::unique_ptr<AttributeBase>> attributes;
    std::unordered_map<std::string, int> map;

    [[nodiscard]] inline AttributeBase* find(const std::string& t_name) {
        auto it = map.find(t_name);
        return it == map.end() ? nullptr : attributes[it->second].get();
    }
};

} // namespace euclide