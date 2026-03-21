project(lumino_base)

#-------------------------------------------------------------------------------
# Sources
#-------------------------------------------------------------------------------
file(GLOB LN_LOCAL_SOURCES
    src/pch.hpp
    src/*.cpp
    src/*.hpp
    include/${PROJECT_NAME}/*.hpp
    include/LuminoBase.hpp
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
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)
target_link_libraries(${PROJECT_NAME} PUBLIC tl::expected)

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------
if(LUMINO_BUILD_TESTS)
    add_executable(lumino_base_test
        test/TestMain.cpp
    )
    target_precompile_headers(lumino_base_test PRIVATE test/pch.hpp)
    target_link_libraries(lumino_base_test PRIVATE lumino_base GTest::gtest)
    add_test(NAME lumino_base_test COMMAND lumino_base_test)
endif()
