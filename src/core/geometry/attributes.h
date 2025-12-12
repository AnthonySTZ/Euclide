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

class Attribute {
    static constexpr int MAX_ATTRIBUTE_SIZE = 4;

  public:
    Attribute(const std::string& t_name, const int t_attrSize, const AttributeType t_attrType);
    Attribute() = default;
    ~Attribute();

    Attribute(const Attribute& t_other);
    Attribute& operator=(const Attribute& t_other);

    [[nodiscard]] inline AttributeType getType() const noexcept { return type; }
    [[nodiscard]] inline int getAttrSize() const noexcept { return attrSize; }

    void resize(const size_t t_size);

    std::unique_ptr<Attribute> clone() const;

  public:
    std::string name;               //< The attribute name
    int attrSize;                   //< The attribute size, i.e float = 1, float3 = 3
    AttributeType type;             //< The attribute type that will be used to determine the type* of the data_ptr
    void* data[MAX_ATTRIBUTE_SIZE]; //< Generic pointers, max 4 so it can handle vec4 at max for SoA and for mat3/4 it
                                    // will use an AoS
    size_t size = 0;

  private:
    void free();
    void copy(const Attribute& t_other);
};

struct AttributeSet {
    size_t size = 0;
    std::vector<std::unique_ptr<Attribute>> attributes;
    std::unordered_map<std::string, int> map;

    AttributeSet() = default;
    ~AttributeSet() = default;

    AttributeSet(const AttributeSet& t_other);
    AttributeSet& operator=(const AttributeSet& t_other);

    [[nodiscard]] Attribute* find(const std::string& t_name);
    Attribute* findOrCreate(const std::string& t_name, const int t_attrSize, const AttributeType t_type);

    void resize(const size_t t_size);

  private:
    void copy(const AttributeSet& t_other);
};

} // namespace euclide