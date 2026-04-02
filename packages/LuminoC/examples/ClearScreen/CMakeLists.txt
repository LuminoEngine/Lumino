project(LuminoC-ClearScreen)
add_executable(${PROJECT_NAME} Main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoC)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "examples")
# Windows: 実行時に LuminoC.dll を見つけられるようコピーする
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:LuminoC>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
