project(lumino_base_test)
    find_package(glm CONFIG REQUIRED)
#-------------------------------------------------------------------------------
# Sources
#-------------------------------------------------------------------------------
file(GLOB LN_LOCAL_SOURCES *.cpp *.hpp)
source_group("" FILES ${LN_LOCAL_SOURCES})

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_executable(
    ${PROJECT_NAME}
    ${LN_LOCAL_SOURCES}
)
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.hpp)
target_link_libraries(${PROJECT_NAME} PRIVATE lumino_base GTest::gtest glm::glm)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
