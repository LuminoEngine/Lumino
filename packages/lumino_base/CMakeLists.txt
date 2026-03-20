project(lumino_base)

file(GLOB LN_LOCAL_SOURCES
    src/*.cpp
    src/*.hpp
    include/${PROJECT_NAME}/*.hpp
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
target_link_libraries(${PROJECT_NAME} PUBLIC tl::expected)
