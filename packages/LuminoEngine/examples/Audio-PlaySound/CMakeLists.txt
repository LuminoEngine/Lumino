project(Audio-PlaySound)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} Main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE "../../../LuminoFFI/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE ASSETS_DIR=U"${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Examples")
