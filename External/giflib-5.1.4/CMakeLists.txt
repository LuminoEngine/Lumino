cmake_minimum_required (VERSION 3.0.0)
project (giflib)

# files
set(LN_LOCAL_SOURCES
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

# create project
add_library(${PROJECT_NAME} STATIC ${LN_LOCAL_SOURCES})

# public include directory
set(LN_DEPENDENCIES_INCLUDE_DIRECTORIES ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} ${PROJECT_SOURCE_DIR} PARENT_SCOPE)
