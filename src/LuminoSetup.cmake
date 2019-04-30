
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 14) # C++14...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

if (DEFINED EMSCRIPTEN)
    list(APPEND CMAKE_FIND_ROOT_PATH ${LUMINO_ENGINE_ROOT}/lib/Emscripten)
    set(CMAKE_EXECUTABLE_SUFFIX .html)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/ImportExternalLibraries.cmake)

#if (DEFINED EMSCRIPTEN)
#	find_package(LuminoCore REQUIRED PATHS ${LUMINO_ENGINE_ROOT}/lib/Emscripten/cmake)
#	find_package(LuminoEngine REQUIRED PATHS ${LUMINO_ENGINE_ROOT}/lib/Emscripten/cmake)
#elseif (ANDROID_ABI)
#	find_package(LuminoCore REQUIRED PATHS ${LUMINO_ENGINE_ROOT}/lib/Android-${ANDROID_ABI}/cmake)
#	find_package(LuminoEngine REQUIRED PATHS ${LUMINO_ENGINE_ROOT}/lib/Android-${ANDROID_ABI}/cmake)
#endif()

message("aaa")

set(_LN_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../include)
set(_LN_LIBRARY_DIR ${CMAKE_CURRENT_LIST_DIR}/../${LN_TARGET_ARCH}-${CMAKE_BUILD_TYPE})

find_library(LuminoCore_LIBRARY_RELEASE NAMES LuminoCore libLuminoCore PATHS ${_LN_LIBRARY_DIR} NO_CMAKE_SYSTEM_PATH)
find_library(LuminoCore_LIBRARY_DEBUG NAMES LuminoCored libLuminoCored PATHS ${_LN_LIBRARY_DIR} NO_CMAKE_SYSTEM_PATH)
add_library(LuminoCore STATIC IMPORTED)
set_target_properties(LuminoCore PROPERTIES IMPORTED_LOCATION_RELEASE "${LuminoCore_LIBRARY_RELEASE}")
set_target_properties(LuminoCore PROPERTIES IMPORTED_LOCATION_DEBUG "${LuminoCore_LIBRARY_DEBUG}")
set_target_properties(LuminoCore PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${_LN_INCLUDE_DIR})

find_library(LuminoEngine_LIBRARY_RELEASE NAMES LuminoEngine libLuminoEngine PATHS ${_LN_LIBRARY_DIR} NO_CMAKE_SYSTEM_PATH)
find_library(LuminoEngine_LIBRARY_DEBUG NAMES LuminoEngined libLuminoEngined PATHS ${_LN_LIBRARY_DIR} NO_CMAKE_SYSTEM_PATH)
add_library(LuminoEngine STATIC IMPORTED)
set_target_properties(LuminoEngine PROPERTIES IMPORTED_LOCATION_RELEASE "${LuminoEngine_LIBRARY_RELEASE}")
set_target_properties(LuminoEngine PROPERTIES IMPORTED_LOCATION_DEBUG "${LuminoEngine_LIBRARY_DEBUG}")
set_target_properties(LuminoEngine PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${_LN_INCLUDE_DIR})
#target_link_libraries(LuminoEngine PRIVATE LuminoCore pcre PNG ZLIB FreeType vorbisfile vorbis ogg Box2D BulletSoftBody BulletDynamics BulletCollision LinearMath tmxlite VulkanHeaders)
set_target_properties(LuminoEngine PROPERTIES INTERFACE_LINK_LIBRARIES "LuminoCore;\$<LINK_ONLY:LuminoCore>;\$<LINK_ONLY:pcre>;\$<LINK_ONLY:PNG>;\$<LINK_ONLY:ZLIB>;\$<LINK_ONLY:FreeType>;\$<LINK_ONLY:vorbisfile>;\$<LINK_ONLY:vorbis>;\$<LINK_ONLY:ogg>;\$<LINK_ONLY:Box2D>;\$<LINK_ONLY:BulletSoftBody>;\$<LINK_ONLY:BulletDynamics>;\$<LINK_ONLY:BulletCollision>;\$<LINK_ONLY:LinearMath>;\$<LINK_ONLY:tmxlite>;\$<LINK_ONLY:VulkanHeaders>")
#find_package(LuminoCore REQUIRED PATHS ${LUMINO_ENGINE_ROOT}/lib/${LN_TARGET_ARCH}-${CMAKE_BUILD_TYPE} PATH_SUFFIXES cmake)
#find_package(LuminoEngine REQUIRED PATHS ${LUMINO_ENGINE_ROOT}/lib/${LN_TARGET_ARCH}-${CMAKE_BUILD_TYPE} PATH_SUFFIXES cmake)

function(ln_add_pch project_name header_file_path source_file_path)

	get_filename_component(header_file_name ${header_file_path} NAME)

	if (MSVC)
		set(ln_compile_flags
			"/Yu\"${header_file_name}\" /FI\"${header_file_name}\""	# use PCH, ForcedIncludeFiles
		)

	else()
		# https://github.com/nanoant/CMakePCHCompiler/blob/master/CMakePCHCompiler.cmake

		get_filename_component(result ${header_file_path} ABSOLUTE)

		# force include header
		set(ln_compile_flags "-include \"${result}\"")
	
	endif()

	# get source files from project (referred LLVM)
	get_property(source_files TARGET ${project_name} PROPERTY SOURCES)

	# set ln_compile_flags to all *.cpp
	foreach (file ${source_files})
		if (${file} MATCHES ".+\\.cpp")
			set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS ${ln_compile_flags})
		endif()
	endforeach()

	# create .pch config
	if (MSVC)
		set_source_files_properties(${source_file_path} PROPERTIES COMPILE_FLAGS "/Yc\"${header_file_name}\"")
	endif()

endfunction()

