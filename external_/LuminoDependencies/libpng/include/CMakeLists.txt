cmake_minimum_required (VERSION 3.0.0)
project (libpng)

set(LN_SOURCES_libpng
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

set(LN_LIB_OUTPUT_DIR "${LN_LIB_OUTPUT_ROOT_DIR}/${PROJECT_NAME}/${LN_LIB_OUTPUT_LOCAL_DIR}")
add_library(${PROJECT_NAME} STATIC ${LN_SOURCES_libpng})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}${LN_STATIC_LIB_POSTFIX}")
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${LN_LIB_OUTPUT_DIR}")

# include dir
include_directories(${PROJECT_SOURCE_DIR}/../zlib-1.2.7)


if (MSVC)
	add_definitions("/wd4996")		# pragma warning disable
endif()

# copy include files
file(COPY "." DESTINATION "${LN_LIB_OUTPUT_ROOT_DIR}/${PROJECT_NAME}/include")
