cmake_minimum_required (VERSION 2.8.7)
project (libpng)

include(../Lumino.Core/cmake/LNCommon.cmake NO_POLICY_SCOPE)

set(LN_LIBPNG_SOURCES
	pngwtran.c
	pngwutil.c
	png.c
	pngerror.c
	pngget.c
	pngmem.c
	pngpread.c
	pngread.c
	pngrio.c
	pngrtran.c
	pngrutil.c
	pngset.c
	pngtrans.c
	pngwio.c
	pngwrite.c)

add_library(${PROJECT_NAME} STATIC ${LN_LIBPNG_SOURCES})

# include dir
include_directories(${PROJECT_SOURCE_DIR}/../zlib)

# Output name setting.
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${LN_LIB_OUTPUT_DIR}")

if (MSVC)
	add_definitions("/wd4996")		# pragma warning disable
endif()

