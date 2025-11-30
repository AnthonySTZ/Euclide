#pragma once

#include <cstdlib>
#include <new> // std::bad_alloc

/// @brief A custom STL-compatible allocator that aligns memory allocations to a specified byte boundary.
///
/// Useful for SIMD operations or other scenarios requiring specific alignment.
template <typename T, size_t Alignment> struct AlignedAllocator {
    /// @brief Type of the objects being allocated.
    using value_type = T;

    /// @brief Default constructor.
    AlignedAllocator() noexcept = default;

    /// @brief Converts from an allocator of another type with the same alignment.
    /// @tparam U Type of the other allocator.
    template <typename U> constexpr AlignedAllocator(const AlignedAllocator<U, Alignment>&) noexcept {}

    /// @brief Allocates memory for n objects of type T, aligned to Alignment bytes.
    /// @param n Number of objects to allocate.
    /// @return Pointer to the aligned memory block.
    /// @throws std::bad_alloc if allocation fails.
    T* allocate(size_t n) {
        void* ptr = nullptr;
#ifdef _WIN32
        ptr = _aligned_malloc(n * sizeof(T), Alignment);
        if (!ptr)
            throw std::bad_alloc();
#else
        if (posix_memalign(&ptr, Alignment, n * sizeof(T)) != 0)
            throw std::bad_alloc();
#endif
        return static_cast<T*>(ptr);
    }

    /// @brief Deallocates memory previously allocated with allocate.
    /// @param p Pointer to the memory block to free.
    /// @param n Number of objects (unused, required by STL interface).
    void deallocate(T* p, size_t) noexcept {
#ifdef _WIN32
        _aligned_free(p);
#else
        free(p);
#endif
    }

    /// @brief Rebind allocator to another type.
    /// @tparam U New type for rebind.
    template <typename U> struct rebind { using other = AlignedAllocator<U, Alignment>; };
};

/// @brief Equality comparison for allocators of potentially different types.
/// @return Always true, as alignment is the same.
template <typename T, typename U, size_t Alignment>
inline bool operator==(const AlignedAllocator<T, Alignment>&, const AlignedAllocator<U, Alignment>&) noexcept {
    return true;
}

/// @brief Inequality comparison for allocators of potentially different types.
/// @return Always false, as equality always returns true.
template <typename T, typename U, size_t Alignment>
inline bool operator!=(const AlignedAllocator<T, Alignment>& a, const AlignedAllocator<U, Alignment>& b) noexcept {
    return !(a == b);
}
