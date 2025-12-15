#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <array>
#include "attribute_types.h"
#include "utils/allocator_utils.h"

namespace euclide {

class Attribute {
  public:
    Attribute(const std::string& t_name) : m_name(t_name) {}
    Attribute() = default;

    Attribute(const Attribute&) = default;
    Attribute& operator=(const Attribute&) = default;

    Attribute(Attribute&&) noexcept = default;
    Attribute& operator=(Attribute&&) noexcept = default;

    virtual ~Attribute() = default;
    virtual void free() = 0;
    virtual std::unique_ptr<Attribute> clone() = 0;

    [[nodiscard]] inline std::string name() const noexcept { return m_name; }
    [[nodiscard]] inline size_t size() const noexcept { return m_size; }

    [[nodiscard]] virtual AttributeType type() const noexcept = 0;
    [[nodiscard]] virtual int attrSize() const noexcept = 0;
    [[nodiscard]] virtual size_t elementSize() const noexcept = 0;

    [[nodiscard]] virtual void* componentRaw(const size_t t_index) = 0;
    [[nodiscard]] virtual const void* componentRaw(const size_t t_index) const = 0;

    [[nodiscard]] inline bool isCompatibleWith(const Attribute& t_other) {
        return type() == t_other.type() && attrSize() == t_other.attrSize();
    }

    virtual void copyAt(const Attribute& t_other, const size_t t_index, const size_t t_numElements) = 0;
    inline void copyAt(const Attribute& t_other, const size_t t_index) { copyAt(t_other, t_index, t_other.size()); }

    template <typename T>
    [[nodiscard]] inline T* component(const size_t t_index) {
        return static_cast<T*>(componentRaw(t_index));
    }

    template <typename T>
    [[nodiscard]] inline const T* component(const size_t t_index) const {
        return static_cast<const T*>(componentRaw(t_index));
    }

    virtual void resize(const size_t t_newSize) = 0;

  protected:
    std::string m_name; //< The attribute name
    size_t m_size = 0;
};

template <typename T, size_t COMPONENTS>
class TypedAttribute : public Attribute {
    using alloc = AlignedAllocator<T, 32>;

  public:
    TypedAttribute(const std::string& t_name) : Attribute(t_name) {}
    ~TypedAttribute();

    // copy
    TypedAttribute(const TypedAttribute& t_other);
    TypedAttribute<T, COMPONENTS>& operator=(const TypedAttribute& t_other);

    // move
    TypedAttribute(TypedAttribute&& t_other) noexcept;
    TypedAttribute<T, COMPONENTS>& operator=(TypedAttribute&& t_other) noexcept;

    void free() override;
    inline std::unique_ptr<Attribute> clone() override {
        return std::make_unique<TypedAttribute<T, COMPONENTS>>(*this);
    }

    inline AttributeType type() const noexcept override { return AttributeTypeOf<T>::value; }
    inline int attrSize() const noexcept override { return COMPONENTS; }
    inline size_t elementSize() const noexcept override { return sizeof(T); }

    inline void* componentRaw(const size_t t_index) override { return m_data[t_index]; }
    inline const void* componentRaw(const size_t t_index) const override { return m_data[t_index]; }

    // TODO: add a capacity and increase more that it needed like vectors for more efficiency
    void resize(const size_t t_newSize) override;

    void copyAt(const Attribute& t_other, const size_t t_index, const size_t t_numElements) override;

  private:
    void copyFrom(const TypedAttribute<T, COMPONENTS>& t_other);

  protected:
    std::array<T*, COMPONENTS> m_data{};
};

// class Attribute {
//     using floatAlloc = AlignedAllocator<float, 32>;

//     static constexpr int MAX_ATTRIBUTE_SIZE = 4;

//   public:
//     Attribute(const std::string& t_name, const int t_attrSize, const AttributeType t_attrType);
//     Attribute() = default;
//     ~Attribute();

//     Attribute(const Attribute& t_other);
//     Attribute& operator=(const Attribute& t_other);

//     [[nodiscard]] inline std::string name() const noexcept { return m_name; }
//     [[nodiscard]] inline AttributeType type() const noexcept { return m_type; }
//     [[nodiscard]] inline int attrSize() const noexcept { return m_attrSize; }
//     [[nodiscard]] inline size_t size() const noexcept { return m_size; }

//     template <typename T>
//     [[nodiscard]] inline T* component(const int t_componentIndex) {
//         if (t_componentIndex >= m_attrSize)
//             return nullptr;
//         return static_cast<T*>(m_data[t_componentIndex]);
//     }

//     void resize(const size_t t_size);

//     std::unique_ptr<Attribute> clone() const;

//   private:
//     std::string m_name;   //< The attribute name
//     int m_attrSize;       //< The attribute size, i.e float = 1, float3 = 3
//     AttributeType m_type; //< The attribute type that will be used to determine the type* of the data_ptr
//     std::array<void*, MAX_ATTRIBUTE_SIZE>
//         m_data{}; //< Generic pointers, max 4 so it can handle vec4 at max for SoA and for mat3/4 it
//                   // will use an AoS
//     size_t m_size = 0;

//   private:
//     void free();
//     void copy(const Attribute& t_other);
// };

} // namespace euclide

#include "attribute_impl.h"