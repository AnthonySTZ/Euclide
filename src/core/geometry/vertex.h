#pragma once

#include <cstdint>

namespace euclide {

/// @brief Represents a vertex in a mesh, referencing a point in the mesh's point array.
struct Vertex {
    uint32_t refPoint = 0; ///< Index of the point in the mesh's point array that this vertex references.
};

} // namespace euclide
