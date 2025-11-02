add_library(core STATIC
    utils/allocator_utils.cpp
    geometry/mesh.cpp
    geometry/points.cpp
    geometry/vertex.cpp
    geometry/primitive.cpp
    nodes/node.cpp
)

target_include_directories(core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# target_link_libraries(core PUBLIC Pythagore)