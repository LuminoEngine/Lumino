﻿
#-------------------------------------------------------------------------------

# Visual Studio ソリューションフォルダを作るようにする
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# サードパーティライブラリ用の共通設定
macro(ln_add_dependencies_common_property_full projectName includeDir)
	set_target_properties(${projectName} PROPERTIES PREFIX "")
	set_target_properties(${projectName} PROPERTIES FOLDER "Dependencies")	# Visual Studio solution folder
	set(LN_DEPENDENCIES_LIBRARIES ${LN_DEPENDENCIES_LIBRARIES} ${projectName})
	set(LN_DEPENDENCIES_INCLUDE_DIRECTORIES ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} ${includeDir})
endmacro()
macro(ln_add_dependencies_common_property projectName)
	set_target_properties(${projectName} PROPERTIES FOLDER "Dependencies")	# Visual Studio solution folder
endmacro()
macro(ln_mark_non_dependencies projectName)
	set_target_properties(${projectName} PROPERTIES FOLDER "Dependencies")	# Visual Studio solution folder
endmacro()

if (MSVC)
	add_definitions("/wd4996")		# pragma warning disable
endif()


set(LN_INCLUDES_gtest
	"${CMAKE_CURRENT_LIST_DIR}/gtest/include"
	"${CMAKE_CURRENT_LIST_DIR}/gtest"
	"${CMAKE_CURRENT_LIST_DIR}/gtest/src")

set(LN_SOURCES_gtest
	"${CMAKE_CURRENT_LIST_DIR}/gtest/src/gtest-all.cc")

#-------------------------------------------------------------------------------

#--------------------------------------
# glfw
if (WIN32)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
	set(GLFW_INSTALL OFF CACHE BOOL "")
	add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/glfw-3.2.1)
	ln_add_dependencies_common_property_full(glfw "${CMAKE_CURRENT_LIST_DIR}/glfw-3.2.1/include")
endif()

#--------------------------------------
# glad
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/glad)
set(LN_DEPENDENCIES_INCLUDE_DIRECTORIES ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} "${CMAKE_CURRENT_LIST_DIR}/glad/include")
ln_add_dependencies_common_property(glad)

#--------------------------------------
# zlib
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/zlib-1.2.11)
ln_add_dependencies_common_property(zlibstatic)
ln_mark_non_dependencies(example)
ln_mark_non_dependencies(minigzip)
ln_mark_non_dependencies(zlib)

#--------------------------------------
# libpng
set(ZLIB_LIBRARY zlibstatic)
set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/zlib-1.2.11 ${CMAKE_CURRENT_BINARY_DIR}/external/zlib-1.2.11) # zlib.h and zconf.h
set(PNG_SHARED OFF CACHE BOOL "")
set(PNG_STATIC ON CACHE BOOL "")
set(PNG_TESTS OFF CACHE BOOL "")
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libpng-1.6.34)
ln_add_dependencies_common_property(png_static)
ln_mark_non_dependencies(genfiles)

