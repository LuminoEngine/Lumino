cmake_minimum_required(VERSION 3.1.0)
project(MeshViewer)

file(GLOB LOCAL_SOURCES_Main "*.cpp" "*.hpp")
source_group("Sources" FILES ${LOCAL_SOURCES_Main})

set(
	LOCAL_SOURCES
	${LOCAL_SOURCES_Main}
)

add_executable(${PROJECT_NAME} ${LOCAL_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${LUMINO_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoEngine-static)
target_compile_definitions(${PROJECT_NAME} PRIVATE LN_GLTF_SAMPLE_MODELS_DIR="${LN_GLTF_SAMPLE_MODELS_DIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Editor")
ln_add_pch(${PROJECT_NAME} "PCH.hpp" "PCH.cpp")
