cmake_minimum_required (VERSION 2.8.7)
project (libogg)

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
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}${LN_LIB_POSTFIX}")

# Copy lib (post build event)
add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> "${CMAKE_SOURCE_DIR}/lib")

if (MSVC)
	add_definitions("/wd4996")		# pragma warning disable
endif()
