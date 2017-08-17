cmake_minimum_required (VERSION 3.0.0)
project (libvorbis)

# files
set(LN_LOCAL_SOURCES
	include/vorbis/vorbisfile.h
	include/vorbis/codec.h
	include/vorbis/vorbisenc.h
	lib/window.h
	lib/analysis.c
	lib/backends.h
	#lib/barkmel.c
	lib/bitrate.c
	lib/bitrate.h
	lib/block.c
	lib/codebook.c
	lib/codebook.h
	lib/codec_internal.h
	lib/envelope.c
	lib/envelope.h
	lib/floor0.c
	lib/floor1.c
	lib/highlevel.h
	lib/info.c
	lib/lookup.c
	lib/lookup.h
	lib/lookup_data.h
	lib/lpc.c
	lib/lpc.h
	lib/lsp.c
	lib/lsp.h
	lib/mapping0.c
	lib/masking.h
	lib/mdct.c
	lib/mdct.h
	lib/misc.h
	lib/os.h
	lib/psy.c
	lib/psy.h
	#lib/psytune.c
	lib/registry.c
	lib/registry.h
	lib/res0.c
	lib/scales.h
	lib/sharedbook.c
	lib/smallft.c
	lib/smallft.h
	lib/synthesis.c
	#lib/tone.c
	lib/vorbisenc.c
	lib/vorbisfile.c
	lib/window.c
)

# create project
add_library(${PROJECT_NAME} STATIC ${LN_LOCAL_SOURCES})

# include dir
include_directories(
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/lib
	${PROJECT_SOURCE_DIR}/../libogg-1.3.2/include
)

if (MSVC)
	add_definitions("/wd4996")		# pragma warning disable
	add_definitions("/wd4244")		# pragma warning disable
	add_definitions("/wd4101")		# pragma warning disable
	add_definitions("/wd4305")		# pragma warning disable
endif()

# public include directory
set(LN_DEPENDENCIES_INCLUDE_DIRECTORIES ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} ${PROJECT_SOURCE_DIR}/include PARENT_SCOPE)
