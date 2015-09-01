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

# include dir
include_directories(${PROJECT_SOURCE_DIR}/../zlib)

# Output name setting.
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}${LN_LIB_POSTFIX}")

# Copy lib (post build event)
add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> "${CMAKE_SOURCE_DIR}/lib")
	
if (MSVC)
	add_definitions("/wd4996")		# pragma warning disable
endif()

