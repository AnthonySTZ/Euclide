#include "attributes.h"

#include <cstring>

namespace euclide {

Attribute::Attribute(const Attribute& t_other) {
    copy(t_other);
}

Attribute& Attribute::operator=(const Attribute& t_other) {
    if (this == &t_other)
        return *this;

    free();
    copy(t_other);
    return *this;
}

Attribute::Attribute(const std::string& t_name, const int t_attrSize, const AttributeType t_attrType)
    : name(t_name), attrSize(t_attrSize), type(t_attrType) {
}

Attribute::~Attribute() {
    free();
}

void Attribute::resize(const size_t t_size) {
    switch (type) {
    case AttributeType::ATTR_TYPE_FLOAT: {
        using floatAlloc = AlignedAllocator<float, 32>;
        for (size_t i = 0; i < attrSize; ++i) {
            float* oldPtr = static_cast<float*>(data[i]);
            float* newPtr = floatAlloc::allocate(t_size);

            if (oldPtr) {
                size_t toCopy = std::min(size, t_size);
                memcpy(newPtr, oldPtr, toCopy * sizeof(float));
            }

            if (t_size > size)
                memset(newPtr + size, 0, (t_size - size) * sizeof(float)); // Init new created float to 0.0f

            if (oldPtr)
                floatAlloc::deallocate(oldPtr);

            data[i] = newPtr;
        }
    } break;
    }

    size = t_size;
}

std::unique_ptr<Attribute> Attribute::clone() const {
    return std::make_unique<Attribute>(*this);
}

void Attribute::free() {
    switch (type) {
    case AttributeType::ATTR_TYPE_FLOAT:
        using floatAlloc = AlignedAllocator<float, 32>;
        for (size_t i = 0; i < MAX_ATTRIBUTE_SIZE; ++i)
            if (data[i])
                floatAlloc::deallocate((float*)data[i], 0);
        break;
    case AttributeType::ATTR_TYPE_STRING:
        std::string* arr = static_cast<std::string*>(data[0]);
        if (arr)
            delete[] arr;
        break;
    }

    size = 0;
}

void Attribute::copy(const Attribute& t_other) {
    free();

    name = t_other.name;
    attrSize = t_other.attrSize;
    type = t_other.type;

    resize(t_other.size);

    switch (type) {
    case AttributeType::ATTR_TYPE_FLOAT: {
        for (size_t c = 0; c < attrSize; ++c) {
            memcpy(data[c], t_other.data[c], sizeof(float) * size);
        }
    }
    }
}

AttributeSet::AttributeSet(const AttributeSet& t_other) {
    copy(t_other);
}

AttributeSet& AttributeSet::operator=(const AttributeSet& t_other) {
    if (this == &t_other)
        return *this;

    attributes.clear();
    map.clear();

    copy(t_other);

    return *this;
}

Attribute* AttributeSet::find(const std::string& t_name) {
    auto it = map.find(t_name);
    return it == map.end() ? nullptr : attributes[it->second].get();
}

Attribute* AttributeSet::findOrCreate(const std::string& t_name, const int t_attrSize, const AttributeType t_type) {
    auto it = map.find(t_name);
    if (it != map.end())
        return attributes[it->second].get();

    size_t attrIndex = attributes.size();
    map.emplace(t_name, attrIndex);
    attributes.emplace_back(std::make_unique<Attribute>(t_name, t_attrSize, t_type));
    auto* attr = attributes[attrIndex].get();
    attr->resize(size);
    return attr;
}

void AttributeSet::resize(const size_t t_size) {
    for (auto& attr : attributes) {
        attr->resize(t_size);
    }
    size = t_size;
}

void AttributeSet::copy(const AttributeSet& t_other) {
    attributes.reserve(t_other.attributes.size());
    for (auto& attr : t_other.attributes)
        attributes.push_back(attr->clone());

    map = t_other.map;
    size = t_other.size;
}

} // namespace euclide