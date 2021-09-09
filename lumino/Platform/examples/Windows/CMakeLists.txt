project(LuminoPlatform-ExampleWindows)

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} WIN32 Main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore LuminoPlatform)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
