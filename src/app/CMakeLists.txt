add_library(app_lib STATIC
    interface/window.cpp
    interface/interface.cpp

    interface/engine.cpp

    interface/panels/panel.cpp
    
    interface/panels/node_graph.cpp
    interface/panels/graph/node_item.cpp
    interface/panels/graph/connection_item.cpp

    interface/panels/viewport.cpp
)

target_include_directories(app_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)


find_package(OpenGL REQUIRED)

target_link_libraries(app_lib PUBLIC core imgui glfw OpenGL::GL glad)


add_executable(app_exec main.cpp)
target_link_libraries(app_exec
    PRIVATE app_lib
)