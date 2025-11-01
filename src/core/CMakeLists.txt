add_library(core STATIC
    utils/alignedAllocator.cpp
    geometry/mesh.cpp
    geometry/points.cpp
)

target_include_directories(core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# target_link_libraries(core PUBLIC Pythagore)