add_library(app_lib STATIC
    interface/window.cpp
)

target_include_directories(app_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)


find_package(OpenGL REQUIRED)

target_link_libraries(app_lib PUBLIC core glfw OpenGL::GL glad)


add_executable(app_exec main.cpp)
target_link_libraries(app_exec
    PRIVATE app_lib
)