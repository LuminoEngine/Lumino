add_executable(hello_triangle HelloTriangle.cpp)
target_link_libraries(hello_triangle PRIVATE LuminoCore LuminoBase LuminoShader)
target_compile_definitions(hello_triangle PRIVATE
    SHADER_FILE="${CMAKE_CURRENT_SOURCE_DIR}/HelloTriangle.slang"
)

add_executable(forward_rendering ForwardRendering.cpp)
target_link_libraries(forward_rendering PRIVATE LuminoCore LuminoBase LuminoShader)

add_executable(hello_mesh HelloMesh.cpp)
target_link_libraries(hello_mesh PRIVATE LuminoCore LuminoBase LuminoShader)

add_executable(hello_texture HelloTexture.cpp)
target_link_libraries(hello_texture PRIVATE LuminoCore LuminoBase LuminoShader)
target_compile_definitions(hello_texture PRIVATE
    ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

add_executable(clear_screen ClearScreen.c)
target_link_libraries(clear_screen PRIVATE LuminoC)
# Windows: 実行時に LuminoC.dll を見つけられるようコピーする
add_custom_command(TARGET clear_screen POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:LuminoC>
        $<TARGET_FILE_DIR:clear_screen>
)
