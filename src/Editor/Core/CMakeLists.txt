cmake_minimum_required (VERSION 3.0.0)
project(LuminoEditor)

set(LN_ROOT_INCLUDE_DIR ${LUMINO_REPO_ROOT_DIR}/include)

file(GLOB LOCAL_HEADERS ${LN_ROOT_INCLUDE_DIR}/LuminoEditor/*.hpp)
source_group("Sources" FILES ${LOCAL_HEADERS})

file(GLOB LN_SOURCES_LuminoEditor *.hpp *.cpp)
source_group("Sources" FILES ${LN_SOURCES_LuminoEditor})

file(GLOB LN_SOURCES_Project Project/*.hpp Project/*.cpp)
source_group("Project" FILES ${LN_SOURCES_Project})

file(GLOB LN_SOURCES_AssetEditor AssetEditor/*.hpp AssetEditor/*.cpp)
source_group("AssetEditor" FILES ${LN_SOURCES_AssetEditor})

file(GLOB LN_SOURCES_StandardPlugin StandardPlugin/*.hpp StandardPlugin/*.cpp)
source_group("StandardPlugin" FILES ${LN_SOURCES_StandardPlugin})

#file(GLOB LN_SOURCES_TilemapPlugin TilemapPlugin/*.hpp TilemapPlugin/*.cpp)
#source_group("TilemapPlugin" FILES ${LN_SOURCES_TilemapPlugin})

#file(GLOB LN_SOURCES_TilemapSceneExtension TilemapSceneExtension/*.hpp TilemapSceneExtension/*.cpp)
#source_group("TilemapSceneExtension" FILES ${LN_SOURCES_TilemapSceneExtension})

file(GLOB LN_SOURCES_Controls Controls/*.hpp Controls/*.cpp)
source_group("Controls" FILES ${LN_SOURCES_Controls})

file(GLOB LN_SOURCES_App App/*.hpp App/*.cpp)
source_group("App" FILES ${LN_SOURCES_App})

set(
	LOCAL_SOURCES
	${LOCAL_HEADERS}
	${LN_SOURCES_LuminoEditor}
	${LN_SOURCES_Project}
	${LN_SOURCES_AssetEditor}
	${LN_SOURCES_StandardPlugin}
	${LN_SOURCES_TilemapPlugin}
	${LN_SOURCES_TilemapSceneExtension}
	${LN_SOURCES_Controls}
	${LN_SOURCES_App}
)

add_library(${PROJECT_NAME} STATIC ${LOCAL_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE . ${LN_ENGINE_INCLUDE_DIR} ${LUMINO_INCLUDE_DIRECTORIES} ${LN_ROOT_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Editor")
ln_add_pch(${PROJECT_NAME} "PCH.hpp" "PCH.cpp")
