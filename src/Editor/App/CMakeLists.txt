cmake_minimum_required(VERSION 3.1.0)
project(LuminoEditorApp)

file(GLOB LOCAL_SOURCES_Main *.cpp *.hpp )
source_group("Sources" FILES ${LOCAL_SOURCES_Main})

set(
	LOCAL_SOURCES
	${LOCAL_SOURCES_Main}
)

add_executable(${PROJECT_NAME} ${LOCAL_SOURCES} )
target_include_directories(${PROJECT_NAME} PRIVATE ${LUMINO_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} LuminoEngine LuminoEditor)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Editor")
ln_add_pch(${PROJECT_NAME} "PCH.hpp" "PCH.cpp")
