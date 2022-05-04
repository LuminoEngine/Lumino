cmake_minimum_required (VERSION 3.0.0)
project(LuminoTranscoder-Core)

file(GLOB LOCAL_SOURCES "*.hpp" "*.cpp")
source_group("Sources" FILES ${LOCAL_SOURCES})

file(GLOB LOCAL_SOURCES_Generators "Generators/*.hpp" "Generators/*.cpp")
source_group("Generators" FILES ${LOCAL_SOURCES_Generators})

file(GLOB LOCAL_SOURCES_Parser "../Parser/ParserIntermediates.hpp" "../Parser/ParserIntermediates.cpp")
source_group("Sources" FILES ${LOCAL_SOURCES_Parser})

add_library(${PROJECT_NAME} STATIC ${LOCAL_SOURCES} ${LOCAL_SOURCES_Generators} ${LOCAL_SOURCES_Parser})
target_include_directories(${PROJECT_NAME} PRIVATE . ${LN_ENGINE_INCLUDE_DIR} ${LUMINO_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Tools")
ln_add_pch(${PROJECT_NAME} "PCH.hpp" "PCH.cpp")

