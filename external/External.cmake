

set(LN_INCLUDES_gtest
	"${CMAKE_CURRENT_LIST_DIR}/gtest/include"
	"${CMAKE_CURRENT_LIST_DIR}/gtest"
	"${CMAKE_CURRENT_LIST_DIR}/gtest/src")

set(LN_SOURCES_gtest
	"${CMAKE_CURRENT_LIST_DIR}/gtest/src/gtest-all.cc")

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/glfw-3.2.1)

