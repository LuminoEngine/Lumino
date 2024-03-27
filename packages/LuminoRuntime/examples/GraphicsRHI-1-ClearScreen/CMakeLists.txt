project(GraphicsRHI-1-ClearScreen)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} Main.cpp)
target_compile_definitions(${PROJECT_NAME} PRIVATE ASSETS_DIR=U"${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore LuminoRuntime Vulkan::Headers)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Examples")

if(LUMINO_USE_WEBGPU)
    target_link_libraries(${PROJECT_NAME} PRIVATE webgpu)
endif()
