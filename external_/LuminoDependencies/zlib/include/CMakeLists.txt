cmake_minimum_required (VERSION 3.0.0)
project (zlib)

set(LN_SOURCES_zlib
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
)

set(LN_LIB_OUTPUT_DIR "${LN_LIB_OUTPUT_ROOT_DIR}/${PROJECT_NAME}/${LN_LIB_OUTPUT_LOCAL_DIR}")
add_library(${PROJECT_NAME} STATIC ${LN_SOURCES_zlib})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}${LN_STATIC_LIB_POSTFIX}")
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${LN_LIB_OUTPUT_DIR}")

if (MSVC)
	add_definitions("/wd4996")		# pragma warning disable
endif()

# copy include files
file(COPY "." DESTINATION "${LN_LIB_OUTPUT_ROOT_DIR}/${PROJECT_NAME}/include")
