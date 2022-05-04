
if(EMSCRIPTEN)
elseif(WIN32 OR APPLE OR UNIX)
    set(LN_OS_DESKTOP ON)
endif()

find_package(fmt CONFIG REQUIRED)

