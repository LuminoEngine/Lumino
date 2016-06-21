cmake_minimum_required (VERSION 2.8.7)
project (libvorbis)

include(../Lumino.Core/cmake/LNCommon.cmake NO_POLICY_SCOPE)

set(LN_SOURCES_libvorbis
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

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES_libvorbis})

# include dir
include_directories(
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/lib
	${PROJECT_SOURCE_DIR}/../libogg/include
)

# Output name setting.
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}${LN_LIB_POSTFIX}")
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${LN_LIB_OUTPUT_DIR}")

if (MSVC)
	add_definitions("/wd4996")		# pragma warning disable
	add_definitions("/wd4244")		# pragma warning disable
	add_definitions("/wd4101")		# pragma warning disable
	add_definitions("/wd4305")		# pragma warning disable
endif()
