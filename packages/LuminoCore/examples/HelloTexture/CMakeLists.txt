project(LuminoCore-HelloTexture)
add_executable(${PROJECT_NAME} Main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore LuminoBase LuminoShader)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../assets"
)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "examples")
