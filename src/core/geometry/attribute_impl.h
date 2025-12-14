#pragma once

#include "attribute.h"

namespace euclide {

template <typename T, size_t COMPONENTS>
inline TypedAttribute<T, COMPONENTS>::~TypedAttribute() {
    free();
}

template <typename T, size_t COMPONENTS>
inline TypedAttribute<T, COMPONENTS>::TypedAttribute(const TypedAttribute& t_other) : Attribute(t_other) {
    copyFrom(t_other);
}

template <typename T, size_t COMPONENTS>
inline TypedAttribute<T, COMPONENTS>& TypedAttribute<T, COMPONENTS>::operator=(const TypedAttribute& t_other) {
    if (this == &t_other)
        return *this;

    copyFrom(t_other);
    return *this;
}

template <typename T, size_t COMPONENTS>
inline void TypedAttribute<T, COMPONENTS>::free() {
    if constexpr (std::is_same_v<T, float>) {
        for (size_t c = 0; c < COMPONENTS; ++c)
            if (m_data[c])
                alloc::deallocate(m_data[c]);
    }

    m_size = 0;
}

template <typename T, size_t COMPONENTS>
inline void TypedAttribute<T, COMPONENTS>::copyFrom(const TypedAttribute<T, COMPONENTS>& t_other) {
    resize(t_other.m_size);

    if constexpr (std::is_same_v<T, float>) {
        for (size_t c = 0; c < COMPONENTS; ++c) {
            memcpy(m_data[c], t_other.m_data[c], sizeof(float) * m_size);
        }
    }
}

template <typename T, size_t COMPONENTS>
inline void TypedAttribute<T, COMPONENTS>::resize(const size_t t_newSize) {
    if constexpr (std::is_same_v<T, float>) {
        for (size_t c = 0; c < COMPONENTS; ++c) {
            T* oldPtr = static_cast<T*>(m_data[c]);
            T* newPtr = alloc::allocate(t_newSize);

            if (oldPtr) {
                size_t toCopy = std::min(m_size, t_newSize);
                memcpy(newPtr, oldPtr, toCopy * sizeof(T));
            }

            if (t_newSize > m_size)
                memset(newPtr + m_size, 0, (t_newSize - m_size) * sizeof(T)); // Init new created float to 0.0f

            if (oldPtr)
                alloc::deallocate(oldPtr);

            m_data[c] = newPtr;
        }
    }

    m_size = t_newSize;
}

} // namespace euclide