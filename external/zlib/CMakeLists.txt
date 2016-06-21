cmake_minimum_required (VERSION 2.8.7)
project (zlib)

include(../Lumino.Core/cmake/LNCommon.cmake NO_POLICY_SCOPE)

set(LN_ZLIB_SOURCES
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
	inflate.c)

add_library(${PROJECT_NAME} STATIC ${LN_ZLIB_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}${LN_LIB_POSTFIX}")
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${LN_LIB_OUTPUT_DIR}")

if (MSVC)
	add_definitions("/wd4996")		# pragma warning disable
endif()

