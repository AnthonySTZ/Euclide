add_library(core STATIC
    utils/allocator_utils.cpp
    utils/timer.cpp

    geometry/mesh.cpp
    geometry/points.cpp
    geometry/vertex.cpp
    geometry/primitive.cpp

    nodes/node.cpp
    nodes/scene.cpp
    nodes/nodes_info.cpp

    fields/node_field.cpp

    nodes/primitive/cube.cpp
    nodes/primitive/grid.cpp
    nodes/primitive/cylinder.cpp
    
    nodes/geometry/transform.cpp
    nodes/geometry/merge.cpp
)

target_include_directories(core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(core PUBLIC)

option(USE_SIMD "Enable SIMD optimizations" OFF)

if (USE_SIMD)
    target_compile_definitions(core PUBLIC USE_SIMD)
    target_compile_options(core PUBLIC -mavx -mfma -msse3)
endif()