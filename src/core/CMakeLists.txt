add_library(core STATIC
    gpu/gpu_device.cpp
    gpu/gpu_buffer.cpp
    gpu/gpu_descriptor.cpp
    gpu/gpu_pipeline.cpp
    gpu/gpu_compute_task.cpp
    gpu/gpu_manager.cpp

    utils/timer.cpp

    axia/tokenizer.cpp
    axia/parser.cpp

    geometry/attribute_set.cpp
    geometry/mesh.cpp

    nodes/node.cpp
    nodes/scene.cpp
    nodes/nodes_info.cpp

    nodes/primitive/cube.cpp
    nodes/primitive/grid.cpp
    nodes/primitive/cylinder.cpp
    nodes/primitive/quad_sphere.cpp
    
    nodes/geometry/transform.cpp
    nodes/geometry/merge.cpp
    nodes/geometry/subdivide.cpp
    nodes/geometry/smooth_normals.cpp
    nodes/geometry/copy_to_points.cpp

    nodes/attributes/attribute_create.cpp
    nodes/attributes/attribute_randomize.cpp
    nodes/attributes/attribute_rename.cpp
    nodes/attributes/attribute_noise.cpp
    nodes/attributes/attribute_math.cpp
    nodes/attributes/attribute_axia.cpp

    nodes/utility/null.cpp
)

target_include_directories(core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# ------------- LINK VULKAN ------------- #
find_package(Vulkan REQUIRED)
target_link_libraries(core PUBLIC Vulkan::Vulkan)

# ------------- COMPILE HLSL SHADERS ------------- #
set(SHADERS
    perlin
)
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/gpu/shaders")
foreach(SHADER ${SHADERS})
    set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/gpu/shaders/${SHADER}.hlsl")
    set(OUT "${CMAKE_BINARY_DIR}/gpu/shaders/${SHADER}.spv")

    add_custom_command(
        OUTPUT ${OUT}
        COMMAND $ENV{VULKAN}/Bin/dxc
                -T cs_6_0
                -E main
                -spirv
                -fvk-use-dx-layout
                -fspv-target-env=vulkan1.1
                -Fo ${OUT}
                ${SRC}
        DEPENDS ${SRC}
        COMMENT "Compiling shader ${SHADER}"
    )

    list(APPEND SPIRV_SHADERS ${OUT})
endforeach()
add_custom_target(Shaders ALL
    DEPENDS ${SPIRV_SHADERS}
)
add_dependencies(core Shaders)

# ---------------- GLOBAL OPTIMIZATION FLAGS ----------------
if (MINGW)
    # High-performance stable flags for GCC MinGW
    target_compile_options(core PRIVATE 
        -O2
        -march=native
        -mtune=native
        -ffast-math
        -funroll-loops
        -fstrict-aliasing
        -fmerge-all-constants
    )
endif()

# ------------- SIMD OPTION ------------- #
option(USE_SIMD "Enable SIMD optimizations" OFF)
if (USE_SIMD)
    target_compile_definitions(core PUBLIC USE_SIMD)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(core PUBLIC -march=native)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(core PUBLIC /arch:AVX2)
    endif()
endif()

# ---------------- OPENMP ----------------
option(USE_OPENMP "Enable OpenMP multithreading" ON)

if (USE_OPENMP)
    find_package(OpenMP REQUIRED)
    target_link_libraries(core PUBLIC OpenMP::OpenMP_CXX)
endif()