cmake_minimum_required (VERSION 3.0.0)
project(lumino)

file(
    GLOB
    LN_SOURCES
    "*.hpp"
    "*.cpp")

source_group("Sources" FILES ${LN_SOURCES})

include_directories(
    .
    ${LUMINO_INCLUDE_DIRECTORIES}
)

add_executable(${PROJECT_NAME} ${LN_SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER=1)
target_link_libraries(${PROJECT_NAME} LuminoEngine LuminoEditor)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Editor")
ln_add_pch(${PROJECT_NAME} "LuminoCLI.PCH.hpp" "LuminoCLI.PCH.cpp")

#------------------------------------------------------------------------------
# Install

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

