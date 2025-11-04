cmake_minimum_required(VERSION 3.12)

project(Butter VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third_party/glfw)
add_subdirectory(third_party/glad)
add_subdirectory(third_party/imgui)
add_subdirectory(third_party/Pythagore)

add_subdirectory(src/core)
add_subdirectory(src/app)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# --- Optional tests
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest/CMakeLists.txt")
    enable_testing()
    add_subdirectory(third_party/googletest)
    add_subdirectory(tests)
endif()