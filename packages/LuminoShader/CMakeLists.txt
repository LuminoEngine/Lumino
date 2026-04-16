project(LuminoShader)

#-------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------
option(LUMINO_USE_SLANG "Enable Slang shader compiler" ON)

#-------------------------------------------------------------------------------
# Dependencies
#-------------------------------------------------------------------------------
find_package(nlohmann_json CONFIG REQUIRED)

#-------------------------------------------------------------------------------
# Sources
#-------------------------------------------------------------------------------
file(GLOB LN_LOCAL_SOURCES
    src/*.cpp
    src/*.hpp
    include/${PROJECT_NAME}/*.hpp
)
file(GLOB LN_LOCAL_SOURCES_shaders
    shaders/*.slang
)
source_group("" FILES ${LN_LOCAL_SOURCES})
source_group("shaders" FILES ${LN_LOCAL_SOURCES_shaders})

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_library(
    ${PROJECT_NAME} STATIC
    ${LN_LOCAL_SOURCES}
    ${LN_LOCAL_SOURCES_shaders}
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)
target_link_libraries(${PROJECT_NAME} PUBLIC LuminoBase)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

if(LUMINO_USE_SLANG)
    target_link_libraries(${PROJECT_NAME} PRIVATE slang::slang)
    target_compile_definitions(${PROJECT_NAME} PUBLIC LUMINO_USE_SLANG)
endif()

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------
if(LUMINO_BUILD_TESTS)
    add_subdirectory(test)
endif()
