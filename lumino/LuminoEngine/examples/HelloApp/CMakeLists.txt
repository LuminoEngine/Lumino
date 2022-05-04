project(Example-Engine-HelloApp)

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} Main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Examples")
