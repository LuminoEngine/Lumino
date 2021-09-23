project(LuminoShaderCompiler-test)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------
file(GLOB LOCAL_SOURCES *.hpp *.cpp)
source_group("Sources" FILES ${LOCAL_SOURCES})

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${LOCAL_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE .)
target_link_libraries(${PROJECT_NAME} LuminoEngine-static LuminoFFI GTest::gtest GTest::gtest_main)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "PCH.hpp" "PCH.cpp")

add_test(NAME ${PROJECT_NAME}-runtest COMMAND ${PROJECT_NAME})
