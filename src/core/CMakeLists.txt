add_library(core STATIC

    gpu/gpu_device.cpp
    gpu/gpu_buffer.cpp
    gpu/gpu_descriptor.cpp
    gpu/gpu_pipeline.cpp
    gpu/gpu_manager.cpp

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

find_package(Vulkan REQUIRED)

target_link_libraries(core PUBLIC Vulkan::Vulkan)

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/gpu/shaders")

add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/gpu/shaders/square.spv"
    COMMAND $ENV{VULKAN}/Bin/dxc
            -T cs_6_0
            -E main
            -spirv
            -fvk-use-dx-layout
            -fspv-target-env=vulkan1.1
            -Fo "${CMAKE_BINARY_DIR}/gpu/shaders/square.spv"
            "${CMAKE_CURRENT_SOURCE_DIR}/gpu/shaders/square.hlsl"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/gpu/shaders/square.hlsl"
    COMMENT "Building Shaders"
)

add_custom_target(ComputeShader
    DEPENDS "${CMAKE_BINARY_DIR}/gpu/shaders/square.spv"
)

add_dependencies(core ComputeShader)

option(USE_SIMD "Enable SIMD optimizations" OFF)
if (USE_SIMD)
    target_compile_definitions(core PUBLIC USE_SIMD)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(core PUBLIC -march=native)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(core PUBLIC /arch:AVX2) # or appropriate MSVC flag
    endif()
endif()
