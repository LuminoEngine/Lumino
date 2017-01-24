# Usage:
#   set(LN_DEPENDENCIES_ROOT_DIR "${PROJECT_SOURCE_DIR}/external/LuminoDependencies/")
#   include(external/LuminoDependencies/LuminoDependencies.cmake)
#	...
#   target_link_libraries(<Project> ${LN_DEPENDENCIES_LIBRARIES})

#------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------

# MSVC Runtime library.
option(LN_MSVC_STATIC_RUNTIME "static runtime (/MT, /MTd) or DLL runtime (/MD, /MDd) or to link either." ON)

# Architecture.
option(LN_BUILD_X64 "Architecture." OFF)

#------------------------------------------------------------------------------
# Apply options
#------------------------------------------------------------------------------
set(LN_STATIC_LIB_POSTFIX "_static")
set(LN_LIB_OUTPUT_ROOT_DIR "${CMAKE_SOURCE_DIR}/Dependencies/")
set(LN_LIB_OUTPUT_LOCAL_DIR "lib/")

if (WIN32)
	# MSVC version
	if (MSVC_VERSION EQUAL 1400)
		set(LN_TARGET_ENV "MSVC80")
	elseif (MSVC_VERSION EQUAL 1500)
		set(LN_TARGET_ENV "MSVC90")
	elseif (MSVC_VERSION EQUAL 1600)
		set(LN_TARGET_ENV "MSVC100")
	elseif (MSVC_VERSION EQUAL 1700)
		set(LN_TARGET_ENV "MSVC110")
	elseif (MSVC_VERSION EQUAL 1800)
		set(LN_TARGET_ENV "MSVC120")
	elseif (MSVC_VERSION EQUAL 1900)
		set(LN_TARGET_ENV "MSVC140")
	endif()
	set(LN_LIB_OUTPUT_LOCAL_DIR "${LN_LIB_OUTPUT_LOCAL_DIR}${LN_TARGET_ENV}")

	# Architecture.
	# http://stackoverflow.com/questions/5334095/cmake-multiarchitecture-compilation
	if (${CMAKE_EXE_LINKER_FLAGS} MATCHES "/machine:x64")	# /machine:x64
		set(LN_ARCH "x64")
	else()
		set(LN_ARCH "x86")
	endif()
	set(LN_LIB_OUTPUT_LOCAL_DIR "${LN_LIB_OUTPUT_LOCAL_DIR}_${LN_ARCH}")

	# Runtime library
	if (LN_MSVC_STATIC_RUNTIME)
		foreach (flag CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
			if (${flag} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
			endif()
			if (${flag} MATCHES "/MDd")
				string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
			endif()
		endforeach()
	endif()
	if (LN_MSVC_STATIC_RUNTIME)
		set(LN_LIB_OUTPUT_LOCAL_DIR "${LN_LIB_OUTPUT_LOCAL_DIR}_MT/")
	else()
		set(LN_LIB_OUTPUT_LOCAL_DIR "${LN_LIB_OUTPUT_LOCAL_DIR}_MD/")
	endif()
endif()


#------------------------------------------------------------------------------
# Create include path, library path
#------------------------------------------------------------------------------
set(LN_DEPENDENCIES_INCLUDES "")

macro(ln_add_dependencis_library2 name libname)
	# http://stackoverflow.com/questions/22592624/force-find-library-to-search-again
	set(LN_DEPENDENCIES_LIBRARIES_DEBUG_LOCAL "${name}-NOTFOUND")
	set(LN_DEPENDENCIES_LIBRARIES_RELEASE_LOCAL "${name}-NOTFOUND")
	
	find_library(
		LN_DEPENDENCIES_LIBRARIES_DEBUG_LOCAL
		NAMES ${libname}
		PATHS "${LN_DEPENDENCIES_ROOT_DIR}${name}/${LN_LIB_OUTPUT_LOCAL_DIR}Debug"
	)
	find_library(
		LN_DEPENDENCIES_LIBRARIES_RELEASE_LOCAL
		NAMES ${libname}
		PATHS "${LN_DEPENDENCIES_ROOT_DIR}${name}/${LN_LIB_OUTPUT_LOCAL_DIR}Release"
	)
	
	list(APPEND LN_DEPENDENCIES_INCLUDES "${LN_DEPENDENCIES_ROOT_DIR}${name}/include")
	list(APPEND LN_DEPENDENCIES_LIBRARIES_DEBUG debug ${LN_DEPENDENCIES_LIBRARIES_DEBUG_LOCAL})
	list(APPEND LN_DEPENDENCIES_LIBRARIES_RELEASE optimized ${LN_DEPENDENCIES_LIBRARIES_RELEASE_LOCAL})
endmacro()

macro(ln_add_dependencis_library name)
	ln_add_dependencis_library2(${name} ${name})
endmacro()

ln_add_dependencis_library(zlib)
ln_add_dependencis_library(libpng)
ln_add_dependencis_library(libogg)
ln_add_dependencis_library(libvorbis)
ln_add_dependencis_library(bullet)
ln_add_dependencis_library(freetype)
ln_add_dependencis_library(giflib)
ln_add_dependencis_library2(effekseer Effekseer)
ln_add_dependencis_library2(effekseer EffekseerRendererDX9)
ln_add_dependencis_library2(effekseer EffekseerRendererGL)
ln_add_dependencis_library2(effekseer EffekseerSoundXAudio2)

set(LN_DEPENDENCIES_LIBRARIES
	${LN_DEPENDENCIES_LIBRARIES_DEBUG}
	${LN_DEPENDENCIES_LIBRARIES_RELEASE}
)

