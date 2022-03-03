project(Example-Graphics-HelloTriangle)

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} Main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore LuminoPlatform LuminoGraphics)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Examples")
