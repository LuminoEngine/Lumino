cmake_minimum_required(VERSION 3.0.0)
project(minizip)

set(LN_PUBLIC_HEADERS
crypt.h
	ioapi.h
	unzip.h
	zip.h
)

set(LN_LOCAL_SOURCES
	ioapi.c
	unzip.c
	zip.c
)

include_directories(${PROJECT_SOURCE_DIR} ${ZLIB_INCLUDE_DIR})

add_library(${PROJECT_NAME} STATIC ${LN_LOCAL_SOURCES})

install(TARGETS ${PROJECT_NAME}
    EXPORT LuminoCoreTargets
    ARCHIVE     DESTINATION lib    # Library installation location (Copied at "INSTALL" build time)
    )

install(FILES ${LN_PUBLIC_HEADERS} DESTINATION "include")
