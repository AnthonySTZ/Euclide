#pragma once

#include <vector>
#include <cstdint>

namespace butter {

/// @brief Represents a polygonal primitive in a mesh.
struct Primitive {
    uint32_t verticesIndex = UINT32_MAX; ///< Index of the first vertex of this primitive in the mesh's vertex array.
    uint32_t numVertices = UINT32_MAX;   ///< Number of vertices that form this primitive.
};

} // namespace butter
