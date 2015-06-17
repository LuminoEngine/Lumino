cmake_minimum_required (VERSION 2.8.7)
project (freetype)

set(LN_FREETYPE_SOURCES
	src/autofit/autofit.c
	src/base/ftadvanc.c
	src/base/ftbbox.c
	src/base/ftbdf.c
	src/base/ftbitmap.c
	src/base/ftcalc.c
	src/base/ftcid.c
	src/base/ftdbgmem.c
	src/base/ftdebug.c
	src/base/ftfntfmt.c
	src/base/ftfstype.c
	src/base/ftgasp.c
	src/base/ftgloadr.c
	src/base/ftglyph.c
	src/base/ftgxval.c
	src/base/ftinit.c
	src/base/ftlcdfil.c
	src/base/ftmm.c
	src/base/ftobjs.c
	src/base/ftotval.c
	src/base/ftoutln.c
	src/base/ftpatent.c
	src/base/ftpfr.c
	src/base/ftrfork.c
	src/base/ftsnames.c
	src/base/ftstream.c
	src/base/ftstroke.c
	src/base/ftsynth.c
	src/base/ftsystem.c
	src/base/fttrigon.c
	src/base/fttype1.c
	src/base/ftutil.c
	src/base/ftwinfnt.c
	src/bdf/bdf.c
	src/bzip2/ftbzip2.c
	src/cache/ftcache.c
	src/cff/cff.c
	src/cid/type1cid.c
	#src/gzip/ftgzip.c
	src/lzw/ftlzw.c
	src/pcf/pcf.c
	src/pfr/pfr.c
	src/psaux/psaux.c
	src/pshinter/pshinter.c
	src/psnames/psmodule.c
	src/raster/raster.c
	src/sfnt/sfnt.c
	src/smooth/smooth.c
	src/truetype/truetype.c
	src/type1/type1.c
	src/type42/type42.c
	src/winfonts/winfnt.c
)

file(GLOB LN_FREETYPE_HEADERS "include/*.h")

add_library(${PROJECT_NAME} STATIC ${LN_FREETYPE_SOURCES} ${LN_FREETYPE_HEADERS})

include_directories("include")
include_directories("src/truetype")
include_directories("src/sfnt")
include_directories("src/autofit")
include_directories("src/smooth")
include_directories("src/raster")
include_directories("src/psaux")
include_directories("src/psnames")

add_definitions(-DFT_DEBUG_LEVEL_ERROR)
add_definitions(-DFT_DEBUG_LEVEL_TRACE)
add_definitions(-DFT2_BUILD_LIBRARY)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}${LN_LIB_POSTFIX}")

add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> "${CMAKE_SOURCE_DIR}/lib")

if (MSVC)
	add_definitions("/wd4996")		# pragma warning disable
endif()
