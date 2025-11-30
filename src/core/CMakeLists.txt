add_library(core STATIC
    utils/timer.cpp

    geometry/mesh.cpp
    geometry/points.cpp

    nodes/node.cpp
    nodes/scene.cpp
    nodes/nodes_info.cpp

    nodes/primitive/cube.cpp
    nodes/primitive/grid.cpp
    nodes/primitive/cylinder.cpp
    
    nodes/geometry/transform.cpp
    nodes/geometry/merge.cpp
    nodes/geometry/subdivide.cpp
)

target_include_directories(core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(core PUBLIC)

option(USE_SIMD "Enable SIMD optimizations" OFF)

if (USE_SIMD)
    target_compile_definitions(core PUBLIC USE_SIMD)
    target_compile_options(core PUBLIC -march=native)
endif()