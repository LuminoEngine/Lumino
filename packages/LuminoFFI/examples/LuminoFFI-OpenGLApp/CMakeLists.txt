project(LuminoFFI-OpenGLApp)
find_package(glfw3 CONFIG REQUIRED)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} Main.cpp)
target_compile_definitions(${PROJECT_NAME} PRIVATE ASSETS_DIR=U"${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoFFI glfw)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Examples")
add_dependencies(${PROJECT_NAME} LuminoFFI)

# Copy LuminoFFI.dll to output directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:LuminoFFI>
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
