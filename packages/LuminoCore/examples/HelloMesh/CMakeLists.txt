project(LuminoCore-HelloMesh)
add_executable(${PROJECT_NAME} Main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore LuminoBase LuminoShader)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "examples")
