
set(ASSETS_DIR "${CMAKE_CURRENT_LIST_DIR}/Assets")

# add_subdirectory(HelloTriangle)
# add_subdirectory(ExternalOpenGLContext)

# if (LN_EMSCRIPTEN)
#     add_subdirectory(NWJSOverlap)
# else()
#     add_subdirectory(Experimental-Rendering)
# endif()

if(LN_OS_DESKTOP)
    add_subdirectory(GraphicsRHI-1-ClearScreen)
    add_subdirectory(GraphicsRHI-HelloTriangle)
endif()
