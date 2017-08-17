cmake_minimum_required (VERSION 3.0.0)
project (libpng)

# files
set(LN_LOCAL_SOURCES
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
	pngwrite.c
)

# create project
add_library(${PROJECT_NAME} STATIC ${LN_LOCAL_SOURCES})

# include dir
include_directories(${PROJECT_SOURCE_DIR}/../zlib-1.2.7)

# public include directory
set(LN_DEPENDENCIES_INCLUDE_DIRECTORIES ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} ${PROJECT_SOURCE_DIR} PARENT_SCOPE)
