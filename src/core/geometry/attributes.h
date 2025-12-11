#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "utils/allocator_utils.h"

namespace euclide {

enum class AttributeType {
    ATTR_TYPE_FLOAT,
    ATTR_TYPE_FLOAT3,
};

struct Attribute {
    std::string name;   //< The attribute name
    int attrSize;       //< The attribute tuple size, i.e float = 1, float3 = 3
    AttributeType type; //< The attribute type that will be used to determine the type* of the data_ptr
    void* data[4]; //< Generic pointers, max 4 so it can handle vec4 at max for SoA and for mat3/4 it will use an AoS

    virtual ~Attribute() = default;
    [[nodiscard]] inline AttributeType getType() const noexcept { return type; }
    [[nodiscard]] inline int getAttrSize() const noexcept { return attrSize; }
};

struct AttributeSet {
    size_t size = 0;
    std::vector<std::unique_ptr<Attribute>> attributes;
    std::unordered_map<std::string, int> map;

    [[nodiscard]] inline Attribute* find(const std::string& t_name) {
        auto it = map.find(t_name);
        return it == map.end() ? nullptr : attributes[it->second].get();
    }
};

} // namespace euclide