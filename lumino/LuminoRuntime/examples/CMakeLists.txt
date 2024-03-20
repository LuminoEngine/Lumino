
set(ASSETS_DIR "${CMAKE_CURRENT_LIST_DIR}/Assets")

add_subdirectory(HelloTriangle)
add_subdirectory(ExternalOpenGLContext)

if (LN_EMSCRIPTEN)
    add_subdirectory(NWJSOverlap)
else()
    add_subdirectory(Experimental-Rendering)
endif()

