#include "attributes.h"

#include <cstring>

namespace euclide {

Attribute::Attribute(const std::string& t_name, const int t_attrSize, const AttributeType t_attrType)
    : m_name(t_name), m_attrSize(t_attrSize), m_type(t_attrType) {
}

// Attribute::Attribute(const Attribute& t_other) {
//     copy(t_other);
// }

// Attribute& Attribute::operator=(const Attribute& t_other) {
//     if (this == &t_other)
//         return *this;

//     free();
//     copy(t_other);
//     return *this;
// }

// Attribute::Attribute(const std::string& t_name, const int t_attrSize, const AttributeType t_attrType)
//     : m_name(t_name), m_attrSize(t_attrSize), m_type(t_attrType) {
// }

// Attribute::~Attribute() {
//     free();
// }

// void Attribute::resize(const size_t t_size) {
//     switch (m_type) {
//     case AttributeType::ATTR_TYPE_FLOAT: {
//         for (size_t i = 0; i < m_attrSize; ++i) {
//             float* oldPtr = static_cast<float*>(m_data[i]);
//             float* newPtr = floatAlloc::allocate(t_size);

//             if (oldPtr) {
//                 size_t toCopy = std::min(m_size, t_size);
//                 memcpy(newPtr, oldPtr, toCopy * sizeof(float));
//             }

//             if (t_size > m_size)
//                 memset(newPtr + m_size, 0, (t_size - m_size) * sizeof(float)); // Init new created float to 0.0f

//             if (oldPtr)
//                 floatAlloc::deallocate(oldPtr);

//             m_data[i] = newPtr;
//         }
//     } break;
//     }

//     m_size = t_size;
// }

// std::unique_ptr<Attribute> Attribute::clone() const {
//     return std::make_unique<Attribute>(*this);
// }

// void Attribute::free() {
//     switch (m_type) {
//     case AttributeType::ATTR_TYPE_FLOAT: {
//         for (size_t i = 0; i < MAX_ATTRIBUTE_SIZE; ++i)
//             if (m_data[i])
//                 floatAlloc::deallocate(static_cast<float*>(m_data[i]), 0);
//     } break;
//     case AttributeType::ATTR_TYPE_STRING: {
//         for (size_t i = 0; i < MAX_ATTRIBUTE_SIZE; ++i) {
//             std::string* arr = static_cast<std::string*>(m_data[0]);
//             if (arr)
//                 delete[] arr;
//         }
//     } break;
//     }

//     m_size = 0;
// }

// void Attribute::copy(const Attribute& t_other) {
//     free();

//     m_name = t_other.m_name;
//     m_attrSize = t_other.m_attrSize;
//     m_type = t_other.m_type;

//     resize(t_other.m_size);

//     switch (m_type) {
//     case AttributeType::ATTR_TYPE_FLOAT: {
//         for (size_t c = 0; c < m_attrSize; ++c) {
//             memcpy(m_data[c], t_other.m_data[c], sizeof(float) * m_size);
//         }
//     }
//     }
// }

AttributeSet::AttributeSet(const AttributeSet& t_other) {
    copy(t_other);
}

AttributeSet& AttributeSet::operator=(const AttributeSet& t_other) {
    if (this == &t_other)
        return *this;

    m_attributes.clear();
    m_map.clear();

    copy(t_other);

    return *this;
}

Attribute* AttributeSet::find(const std::string& t_name) {
    auto it = m_map.find(t_name);
    return it == m_map.end() ? nullptr : m_attributes[it->second].get();
}

Attribute* AttributeSet::findOrCreate(const std::string& t_name, const int t_attrSize, const AttributeType t_type) {
    auto it = m_map.find(t_name);
    if (it != m_map.end())
        return m_attributes[it->second].get();

    size_t attrIndex = m_attributes.size();
    m_map.emplace(t_name, attrIndex);
    m_attributes.emplace_back(std::make_unique<Attribute>(t_name, t_attrSize, t_type));
    auto* attr = m_attributes[attrIndex].get();
    attr->resize(m_size);
    return attr;
}

Attribute* AttributeSet::get(const size_t t_index) {
    if (t_index >= m_attributes.size())
        return nullptr;

    return m_attributes[t_index].get();
}

void AttributeSet::resize(const size_t t_size) {
    for (auto& attr : m_attributes) {
        attr->resize(t_size);
    }
    m_size = t_size;
}

void AttributeSet::copy(const AttributeSet& t_other) {
    m_attributes.reserve(t_other.m_attributes.size());
    for (auto& attr : t_other.m_attributes)
        m_attributes.push_back(attr->clone());

    m_map = t_other.m_map;
    m_size = t_other.m_size;
}

} // namespace euclide