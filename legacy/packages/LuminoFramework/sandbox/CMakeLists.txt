cmake_minimum_required (VERSION 3.0.0)
project(LuminoFramework-sandbox)

file(GLOB LOCAL_SOURCES *.cpp *.hpp)

include_directories(
	${LUMINO_INCLUDE_DIRECTORIES}
)

add_executable(${PROJECT_NAME} ${LOCAL_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoFramework imgui::imgui)
target_compile_definitions(${PROJECT_NAME} PRIVATE LN_GLTF_SAMPLE_MODELS_DIR="${LN_GLTF_SAMPLE_MODELS_DIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
