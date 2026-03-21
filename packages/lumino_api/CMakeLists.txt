project(lumino_api)

#-------------------------------------------------------------------------------
# Sources
#-------------------------------------------------------------------------------
file(GLOB LN_LOCAL_SOURCES
    src/pch.hpp
    src/*.cpp
    src/*.hpp
    include/${PROJECT_NAME}/*.h
)
source_group("" FILES ${LN_LOCAL_SOURCES})

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_library(
    ${PROJECT_NAME} SHARED
    ${LN_LOCAL_SOURCES}
)
target_compile_definitions(${PROJECT_NAME} PRIVATE LUMINO_BUILD_DLL)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    lumino_base
    lumino_core)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------
if(LUMINO_BUILD_TESTS)
    add_subdirectory(test)
endif()
