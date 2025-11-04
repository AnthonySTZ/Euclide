cmake_minimum_required(VERSION 3.15)
project(glad)

# List all source files
set(GLAD_SOURCES
    include/glad/glad.h
    src/glad.c
    include/KHR/khrplatform.h
)

# Create a static library
add_library(glad STATIC ${GLAD_SOURCES})

# Include directories
target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/KHR
)
