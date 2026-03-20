add_executable(hello_triangle HelloTriangle.cpp)
target_link_libraries(hello_triangle PRIVATE lumino_core lumino_base lumino_shader)
target_compile_definitions(hello_triangle PRIVATE
    SHADER_FILE="${CMAKE_CURRENT_SOURCE_DIR}/HelloTriangle.slang"
)
