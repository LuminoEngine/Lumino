add_executable(hello_triangle HelloTriangle.cpp)
target_link_libraries(hello_triangle PRIVATE lumino_core lumino_base lumino_shader)
target_compile_definitions(hello_triangle PRIVATE
    SHADER_FILE="${CMAKE_CURRENT_SOURCE_DIR}/HelloTriangle.slang"
)

add_executable(forward_rendering ForwardRendering.cpp)
target_link_libraries(forward_rendering PRIVATE lumino_core lumino_base lumino_shader)

add_executable(hello_mesh HelloMesh.cpp)
target_link_libraries(hello_mesh PRIVATE lumino_core lumino_base lumino_shader)

add_executable(clear_screen ClearScreen.c)
target_link_libraries(clear_screen PRIVATE lumino_api)
# Windows: 実行時に lumino_api.dll を見つけられるようコピーする
add_custom_command(TARGET clear_screen POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:lumino_api>
        $<TARGET_FILE_DIR:clear_screen>
)
