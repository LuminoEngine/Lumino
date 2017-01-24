cmake_minimum_required (VERSION 3.0.0)
project (giflib)

set(LN_SOURCES_giflib
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

set(LN_LIB_OUTPUT_DIR "${LN_LIB_OUTPUT_ROOT_DIR}/${PROJECT_NAME}/${LN_LIB_OUTPUT_LOCAL_DIR}")
add_library(${PROJECT_NAME} STATIC ${LN_SOURCES_giflib})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}${LN_STATIC_LIB_POSTFIX}")
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${LN_LIB_OUTPUT_DIR}")

if (MSVC)
	add_definitions("/wd4996")		# pragma warning disable
endif()

# copy include files
file(COPY "." DESTINATION "${LN_LIB_OUTPUT_ROOT_DIR}/${PROJECT_NAME}/include")
