
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

#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/zlib-1.2.11)
#set(LN_DEPENDENCIES_INCLUDE_DIRECTORIES ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} "${CMAKE_CURRENT_LIST_DIR}/zlib-1.2.11")
#ln_add_dependencies_common_property(zlibstatic)
#ln_mark_non_dependencies(example)
#ln_mark_non_dependencies(minigzip)
#ln_mark_non_dependencies(zlib)
#message(${CMAKE_CURRENT_BINARY_DIR}/ExternalInstall/zlib/lib)
set(ZLIB_ROOT ${CMAKE_CURRENT_BINARY_DIR}/ExternalInstall/zlib)
#set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/ExternalInstall")
find_package(ZLIB REQUIRED)
#find_library(zlibstatic_LIB NAMES zlibstaticd PATHS ${CMAKE_CURRENT_BINARY_DIR}/ExternalInstall/zlib/lib)
#message(${zlibstatic_LIB})
#message(${ZLIB_FOUND})
#message(${ZLIB_INCLUDE_DIRS})
#message(${ZLIB_LIBRARIES})
#message(${ZLIB_STATIC_LIB})




#--------------------------------------
# libpng

set(PNG_ROOT ${CMAKE_CURRENT_BINARY_DIR}/ExternalInstall/libpng)
set(PNG_DIR ${PNG_ROOT})
#set(CMAKE_PREFIX_PATH ${PNG_ROOT})
find_package(PNG REQUIRED)
#find_package(PNG REQUIRED PATHS ${CMAKE_CURRENT_BINARY_DIR}/ExternalInstall/libpng)
#find_library (PNG_LIB NAMES libpng libpng16_static PATHS ${PNG_ROOT})
#message(${PNG_LIB})
#message(${PNG_LIBRARIES})

#set(ZLIB_LIBRARY ${ZLIB_LIBRARIES})
#set(ZLIB_INCLUDE_DIR ${ZLIB_INCLUDE_DIRS}) # zlib.h and zconf.h
#set(PNG_SHARED OFF CACHE BOOL "")
#set(PNG_STATIC ON CACHE BOOL "")
#set(PNG_TESTS OFF CACHE BOOL "")

#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libpng-1.6.34)
#set(LN_DEPENDENCIES_INCLUDE_DIRECTORIES ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} "${CMAKE_CURRENT_LIST_DIR}/libpng-1.6.34")
#ln_add_dependencies_common_property(png_static)
#ln_mark_non_dependencies(genfiles)


#--------------------------------------
# glslang

set(GLSLANG_ROOT ${CMAKE_CURRENT_BINARY_DIR}/ExternalInstall/glslang)

find_library(glslang_LIBRARY_RELEASE NAMES glslang PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(glslang_LIBRARY_DEBUG NAMES glslangd PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(HLSL_LIBRARY_RELEASE NAMES HLSL PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(HLSL_LIBRARY_DEBUG NAMES HLSLd PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(OGLCompiler_LIBRARY_RELEASE NAMES OGLCompiler PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(OGLCompiler_LIBRARY_DEBUG NAMES OGLCompilerd PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(OSDependent_LIBRARY_RELEASE NAMES OSDependent PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(OSDependent_LIBRARY_DEBUG NAMES OSDependentd PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(SPIRV_LIBRARY_RELEASE NAMES SPIRV PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(SPIRV_LIBRARY_DEBUG NAMES SPIRVd PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(SPVRemapper_LIBRARY_RELEASE NAMES SPVRemapper PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(SPVRemapper_LIBRARY_DEBUG NAMES SPVRemapperd PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)

set(glslang_INCLUDE_DIRS "${GLSLANG_ROOT}/include")

set(LIB_NAME glslang)
add_library(${LIB_NAME} STATIC IMPORTED)
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
set(glslang_LIBRARIES ${glslang_LIBRARIES} ${LIB_NAME})

set(LIB_NAME HLSL)
add_library(${LIB_NAME} STATIC IMPORTED)
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
set(glslang_LIBRARIES ${glslang_LIBRARIES} ${LIB_NAME})

set(LIB_NAME OGLCompiler)
add_library(${LIB_NAME} STATIC IMPORTED)
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
set(glslang_LIBRARIES ${glslang_LIBRARIES} ${LIB_NAME})

set(LIB_NAME OSDependent)
add_library(${LIB_NAME} STATIC IMPORTED)
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
set(glslang_LIBRARIES ${glslang_LIBRARIES} ${LIB_NAME})

set(LIB_NAME SPIRV)
add_library(${LIB_NAME} STATIC IMPORTED)
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
set(glslang_LIBRARIES ${glslang_LIBRARIES} ${LIB_NAME})

set(LIB_NAME SPVRemapper)
add_library(${LIB_NAME} STATIC IMPORTED)
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
set(glslang_LIBRARIES ${glslang_LIBRARIES} ${LIB_NAME})







#--------------------------------------
# SPIRV-Cross

set(SPIRV-Cross_ROOT ${CMAKE_CURRENT_BINARY_DIR}/ExternalInstall/SPIRV-Cross)

find_library(spirv-cross-core_LIBRARY_RELEASE NAMES spirv-cross-core PATHS ${SPIRV-Cross_ROOT} PATH_SUFFIXES lib)
find_library(spirv-cross-core_LIBRARY_DEBUG NAMES spirv-cross-cored PATHS ${SPIRV-Cross_ROOT} PATH_SUFFIXES lib)
find_library(spirv-cross-glsl_LIBRARY_RELEASE NAMES spirv-cross-glsl PATHS ${SPIRV-Cross_ROOT} PATH_SUFFIXES lib)
find_library(spirv-cross-glsl_LIBRARY_DEBUG NAMES spirv-cross-glsld PATHS ${SPIRV-Cross_ROOT} PATH_SUFFIXES lib)

set(LIB_NAME spirv-cross-core)
add_library(${LIB_NAME} STATIC IMPORTED)
#set_property(TARGET spirv-cross-core APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")

add_library(spirv-cross-glsl STATIC IMPORTED)
#set_property(TARGET spirv-cross-glsl APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(spirv-cross-glsl PROPERTIES IMPORTED_LOCATION_RELEASE "${spirv-cross-glsl_LIBRARY_RELEASE}")
set_target_properties(spirv-cross-glsl PROPERTIES IMPORTED_LOCATION_DEBUG "${spirv-cross-glsl_LIBRARY_DEBUG}")

set(spirv-cross_INCLUDE_DIRS "${SPIRV-Cross_ROOT}/include")
set(spirv-cross_LIBRARIES spirv-cross-core spirv-cross-glsl)
