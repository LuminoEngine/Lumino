project(lumino_base)

#-------------------------------------------------------------------------------
# Sources
#-------------------------------------------------------------------------------
file(GLOB LN_LOCAL_SOURCES
    include/LuminoBase.hpp
    include/${PROJECT_NAME}/*.hpp
    src/*.cpp
    src/*.hpp
    src/pch.hpp
)
file(GLOB LN_LOCAL_SOURCES_math
    include/${PROJECT_NAME}/math/*.hpp
    src/math/*.cpp
    src/math/*.hpp
)
source_group("" FILES ${LN_LOCAL_SOURCES})
source_group("math" FILES ${LN_LOCAL_SOURCES_math})

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_library(
    ${PROJECT_NAME} STATIC
    ${LN_LOCAL_SOURCES}
    ${LN_LOCAL_SOURCES_math}
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)
target_link_libraries(${PROJECT_NAME} PUBLIC tl::expected)

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------
if(LUMINO_BUILD_TESTS)
    add_subdirectory(test)
endif()
