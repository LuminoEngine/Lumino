project(lumino_core)

file(GLOB LN_LOCAL_SOURCES_platform
    src/platform/*.cpp
    src/platform/*.hpp
    include/${PROJECT_NAME}/platform/*.hpp
)
source_group("platform" FILES ${LN_LOCAL_SOURCES_platform})

set(LN_LOCAL_SOURCES
    ${LN_LOCAL_SOURCES_platform}
)

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_library(
    ${PROJECT_NAME} STATIC
    ${LN_LOCAL_SOURCES}
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME} PUBLIC lumino_base)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
