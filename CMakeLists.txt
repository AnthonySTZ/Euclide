cmake_minimum_required(VERSION 3.12)

project(Butter VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third_party/Pythagore)
add_subdirectory(src/core)

add_executable(Butter app/main.cpp)

target_link_libraries(Butter PRIVATE core)