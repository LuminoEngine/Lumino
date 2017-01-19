cmake_minimum_required (VERSION 2.8.7)
project (libogg)

include(../Lumino.Core/cmake/LNCommon.cmake NO_POLICY_SCOPE)

set(LN_SOURCES_libogg
	include/ogg/os_types.h
	include/ogg/config_types.h
	include/ogg/ogg.h
	src/bitwise.c
	src/framing.c
)

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES_libogg})

# include dir
include_directories(
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/include
)

# Output name setting.
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${LN_LIB_OUTPUT_DIR}")

if (MSVC)
	add_definitions("/wd4996")		# pragma warning disable
endif()
