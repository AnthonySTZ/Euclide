cmake_minimum_required(VERSION 3.15)
project(imgui)

# List all source files
set(IMGUI_SOURCES
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_opengl3.cpp
)

# Create a static library
add_library(imgui STATIC ${IMGUI_SOURCES})

# Include directories
target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)

target_link_libraries(imgui PRIVATE glfw)