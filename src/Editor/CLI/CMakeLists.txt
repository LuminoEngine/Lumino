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
target_precompile_headers(${PROJECT_NAME} PRIVATE "LuminoCLI.PCH.hpp")

#------------------------------------------------------------------------------
# Install

if (LUMINO_INSTALL)
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
endif()

