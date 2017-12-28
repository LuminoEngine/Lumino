cmake_minimum_required(VERSION 3.0.0)
project(zlib)

# files
set(LN_LOCAL_SOURCES
	inftrees.c
	trees.c
	uncompr.c
	zutil.c
	adler32.c
	compress.c
	crc32.c
	deflate.c
	gzclose.c
	gzlib.c
	gzread.c
	gzwrite.c
	infback.c
	inffast.c
	inflate.c
	contrib/minizip/zip.c
	contrib/minizip/ioapi.c
	contrib/minizip/unzip.c
)

# create project
add_library(${PROJECT_NAME} STATIC ${LN_LOCAL_SOURCES})

# include dir
include_directories(${PROJECT_SOURCE_DIR})

# public include directory
set(LN_DEPENDENCIES_INCLUDE_DIRECTORIES ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} ${PROJECT_SOURCE_DIR} PARENT_SCOPE)
