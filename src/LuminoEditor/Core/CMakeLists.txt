cmake_minimum_required (VERSION 3.0.0)
project(EditorCore)

set(LN_ROOT_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../include)

file(GLOB LOCAL_HEADERS ${LN_ROOT_INCLUDE_DIR}/LuminoEditor/*.hpp)
source_group("Sources" FILES ${LOCAL_HEADERS})

file(GLOB LN_SOURCES_EditorCore *.hpp *.cpp)
source_group("Sources" FILES ${LN_SOURCES_EditorCore})

file(GLOB LN_SOURCES_StandardPlugin StandardPlugin/*.hpp StandardPlugin/*.cpp)
source_group("StandardPlugin" FILES ${LN_SOURCES_StandardPlugin})

file(GLOB LN_SOURCES_Controls Controls/*.hpp Controls/*.cpp)
source_group("Controls" FILES ${LN_SOURCES_Controls})

file(GLOB LN_SOURCES_App App/*.hpp App/*.cpp)
source_group("App" FILES ${LN_SOURCES_App})

set(
	LOCAL_SOURCES
	${LOCAL_HEADERS}
	${LN_SOURCES_EditorCore}
	${LN_SOURCES_StandardPlugin}
	${LN_SOURCES_Controls}
	${LN_SOURCES_App}
)

add_library(${PROJECT_NAME} STATIC ${LOCAL_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE . ${LN_ENGINE_INCLUDE_DIR} ${LUMINO_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Editor")
ln_add_pch(${PROJECT_NAME} "PCH.hpp" "PCH.cpp")
