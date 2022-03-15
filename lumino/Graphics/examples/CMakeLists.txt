
set(ASSETS_DIR "${CMAKE_CURRENT_LIST_DIR}/Assets")

if (LN_EMSCRIPTEN)
    add_subdirectory(WebGL)
endif()
