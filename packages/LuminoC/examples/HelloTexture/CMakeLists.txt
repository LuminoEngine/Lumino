project(LuminoC-HelloTexture)
add_executable(${PROJECT_NAME} Main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw LuminoC)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../assets"
)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "examples")
# Windows: 実行時に LuminoC.dll を見つけられるようコピーする
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:LuminoC>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_dependencies(${PROJECT_NAME} LuminoC)
