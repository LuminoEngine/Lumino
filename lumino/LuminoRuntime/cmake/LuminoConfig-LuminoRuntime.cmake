
find_package(yaml-cpp CONFIG REQUIRED)

#if (NOT DEFINED EMSCRIPTEN)
#    find_package(CURL CONFIG REQUIRED)
#endif()

if (LN_OS_DESKTOP)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(glad CONFIG REQUIRED)
endif()

