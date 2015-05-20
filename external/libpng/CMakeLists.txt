cmake_minimum_required (VERSION 2.8.7)
project (libpng)

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
include_directories(${PROJECT_SOURCE_DIR}/../zlib)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}${LN_LIB_POSTFIX}")

if (MSVC)
	add_definitions("/wd4996")		# pragma warning disable
endif()

