add_library(app_lib STATIC
    interface/window.cpp
    interface/interface.cpp

    interface/engine.cpp

    interface/panels/panel.cpp
    
    interface/panels/node_graph_panel.cpp

    interface/panels/graph/node_graph.cpp
    interface/panels/graph/node_graph_renderer.cpp
    interface/panels/graph/node_graph_interaction.cpp
    interface/panels/graph/node_graph_input_handler.cpp
    interface/panels/graph/node_graph_menu.cpp

    interface/panels/graph/node_item.cpp
    interface/panels/graph/connection_item.cpp
    interface/panels/graph/cutting_line.cpp
    
    interface/panels/viewport_panel.cpp
    interface/panels/viewport/viewport.cpp
    interface/panels/viewport/viewport_renderer.cpp
    interface/panels/viewport/viewport_input_handler.cpp
    interface/panels/viewport/rendering/renderer.cpp
    interface/panels/viewport/rendering/framebuffer.cpp
    interface/panels/viewport/rendering/shader_program.cpp
    interface/panels/viewport/rendering/render_model.cpp
    interface/panels/viewport/rendering/camera.cpp
    
    interface/panels/parameters_panel.cpp
    interface/panels/parameters/parameters.cpp
    interface/panels/parameters/parameters_renderer.cpp
    interface/panels/parameters/fields/field_drawer.cpp


    )

target_include_directories(app_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)


find_package(OpenGL REQUIRED)

target_link_libraries(app_lib PUBLIC core imgui glfw OpenGL::GL glad glm)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/interface/panels/viewport/rendering/shaders DESTINATION ${CMAKE_BINARY_DIR})

add_executable(app_exec main.cpp)
target_link_libraries(app_exec
    PRIVATE app_lib
)