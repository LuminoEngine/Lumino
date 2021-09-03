cmake_minimum_required (VERSION 3.0.0)
project(LuminoEngine-sandbox)

file(GLOB LOCAL_SOURCES *.cpp *.hpp)

add_executable(${PROJECT_NAME} ${LOCAL_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${LN_VCPKG_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoEngine-static LuminoCore)
target_compile_definitions(${PROJECT_NAME} PRIVATE LN_GLTF_SAMPLE_MODELS_DIR="${LN_GLTF_SAMPLE_MODELS_DIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
