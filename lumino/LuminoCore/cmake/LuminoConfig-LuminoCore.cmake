
set(LN_LINK_CMAKE_SHARED_LIB_PACKAGE OFF)
if (MSVC)
    if (NOT LN_MSVC_STATIC_RUNTIME)
        set(LN_LINK_CMAKE_SHARED_LIB_PACKAGE ON)
    endif()
endif()



if(EMSCRIPTEN)
elseif(WIN32 OR APPLE OR UNIX)
    set(LN_OS_DESKTOP ON)
endif()

find_package(fmt CONFIG REQUIRED)

