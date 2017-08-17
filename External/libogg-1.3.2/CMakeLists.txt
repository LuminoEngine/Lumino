cmake_minimum_required (VERSION 3.0.0)
project (libogg)

# files
set(LN_LOCAL_SOURCES
	include/ogg/os_types.h
	include/ogg/config_types.h
	include/ogg/ogg.h
	src/bitwise.c
	src/framing.c
)

add_library(${PROJECT_NAME} STATIC ${LN_LOCAL_SOURCES})

# include dir
include_directories(
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/include
)

# public include directory
set(LN_DEPENDENCIES_INCLUDE_DIRECTORIES ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} ${PROJECT_SOURCE_DIR}/include PARENT_SCOPE)
