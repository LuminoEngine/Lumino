project(LuminoGraphics-sandbox)

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} Main.cpp)
target_compile_definitions(${PROJECT_NAME} PRIVATE ASSETS_DIR=U"${CMAKE_CURRENT_LIST_DIR}/Assets")
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore LuminoPlatform LuminoGraphics LuminoShaderCompiler LuminoBitmap)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
