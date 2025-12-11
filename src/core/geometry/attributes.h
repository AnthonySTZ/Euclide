#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "utils/allocator_utils.h"

namespace euclide {

enum class AttributeType {
    ATTR_TYPE_FLOAT,
    ATTR_TYPE_STRING,
};

struct Attribute {
    std::string name;   //< The attribute name
    int attrSize;       //< The attribute size, i.e float = 1, float3 = 3
    AttributeType type; //< The attribute type that will be used to determine the type* of the data_ptr
    void* data[4]; //< Generic pointers, max 4 so it can handle vec4 at max for SoA and for mat3/4 it will use an AoS
    size_t size = 0;

    ~Attribute();
    [[nodiscard]] inline AttributeType getType() const noexcept { return type; }
    [[nodiscard]] inline int getAttrSize() const noexcept { return attrSize; }

    void resize(const size_t t_size);
};

struct AttributeSet {
    size_t size = 0;
    std::vector<std::unique_ptr<Attribute>> attributes;
    std::unordered_map<std::string, int> map;

    [[nodiscard]] inline Attribute* find(const std::string& t_name) {
        auto it = map.find(t_name);
        return it == map.end() ? nullptr : attributes[it->second].get();
    }

    inline Attribute* findOrCreate(const std::string& t_name, int t_size, AttributeType t_type) {
        auto it = map.find(t_name);
        if (it != map.end())
            return attributes[it->second].get();

        size_t attrIndex = attributes.size();
        map.emplace(t_name, attrIndex);
        attributes.emplace_back(std::make_unique<Attribute>(t_name, t_size, t_type));
        auto* attr = attributes[attrIndex].get();
        attr->resize(size);
        return attr;
    }
};

} // namespace euclide