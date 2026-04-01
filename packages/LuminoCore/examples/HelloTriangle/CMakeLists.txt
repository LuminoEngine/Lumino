project(LuminoCore-HelloTriangle)
add_executable(${PROJECT_NAME} Main.cpp HelloTriangle.slang)
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore LuminoBase LuminoShader)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    SHADER_FILE="${CMAKE_CURRENT_SOURCE_DIR}/HelloTriangle.slang"
)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "examples")
