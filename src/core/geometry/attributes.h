#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <array>
#include "utils/allocator_utils.h"

namespace euclide {

enum class AttributeType {
    ATTR_TYPE_FLOAT,
    ATTR_TYPE_STRING,
};

class Attribute {
    using floatAlloc = AlignedAllocator<float, 32>;

    static constexpr int MAX_ATTRIBUTE_SIZE = 4;

  public:
    Attribute(const std::string& t_name, const int t_attrSize, const AttributeType t_attrType);
    Attribute() = default;
    ~Attribute();

    Attribute(const Attribute& t_other);
    Attribute& operator=(const Attribute& t_other);

    [[nodiscard]] inline std::string name() const noexcept { return m_name; }
    [[nodiscard]] inline AttributeType type() const noexcept { return m_type; }
    [[nodiscard]] inline int attrSize() const noexcept { return m_attrSize; }
    [[nodiscard]] inline size_t size() const noexcept { return m_size; }

    template <typename T>
    [[nodiscard]] inline T* component(const int t_componentIndex) {
        if (t_componentIndex >= m_attrSize)
            return nullptr;
        return static_cast<T*>(m_data[t_componentIndex]);
    }

    void resize(const size_t t_size);

    std::unique_ptr<Attribute> clone() const;

  private:
    std::string m_name;   //< The attribute name
    int m_attrSize;       //< The attribute size, i.e float = 1, float3 = 3
    AttributeType m_type; //< The attribute type that will be used to determine the type* of the data_ptr
    std::array<void*, MAX_ATTRIBUTE_SIZE>
        m_data{}; //< Generic pointers, max 4 so it can handle vec4 at max for SoA and for mat3/4 it
                  // will use an AoS
    size_t m_size = 0;

  private:
    void free();
    void copy(const Attribute& t_other);
};

class AttributeSet {
  public:
    AttributeSet() = default;
    ~AttributeSet() = default;

    AttributeSet(const AttributeSet& t_other);
    AttributeSet& operator=(const AttributeSet& t_other);

    [[nodiscard]] Attribute* find(const std::string& t_name);
    Attribute* findOrCreate(const std::string& t_name, const int t_attrSize, const AttributeType t_type);

    void resize(const size_t t_size);
    [[nodiscard]] inline size_t size() const noexcept { return m_size; }

  private:
    size_t m_size = 0;
    std::vector<std::unique_ptr<Attribute>> m_attributes;
    std::unordered_map<std::string, int> m_map;

  private:
    void copy(const AttributeSet& t_other);
};

} // namespace euclide