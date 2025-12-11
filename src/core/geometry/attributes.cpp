#include "attributes.h"

#include <cstring>

namespace euclide {

Attribute::~Attribute() {
    switch (type) {
    case AttributeType::ATTR_TYPE_FLOAT:
        for (size_t i = 0; i < attrSize; ++i)
            if (data[i])
                AlignedAllocator<float, 32>().deallocate((float*)data[i], 0);
        break;
    case AttributeType::ATTR_TYPE_STRING:
        std::string* arr = static_cast<std::string*>(data[0]);
        if (arr)
            delete[] arr;
        break;
    }
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

} // namespace euclide