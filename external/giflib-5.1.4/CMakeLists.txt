cmake_minimum_required (VERSION 2.8.7)
project (giflib)

set(LN_giflib_Sources
	quantize.c
	dgif_lib.c
	egif_lib.c
	gif_err.c
	gif_font.c
	gif_hash.c
	gif_hash.h
	gif_lib.h
	gif_lib_private.h
	gifalloc.c
	openbsd-reallocarray.c
)

add_library(${PROJECT_NAME} STATIC ${LN_giflib_Sources})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}${LN_LIB_POSTFIX}")

add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> "${CMAKE_SOURCE_DIR}/lib")

if (MSVC)
	add_definitions("/wd4996")		# pragma warning disable
endif()

