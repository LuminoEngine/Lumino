
set(ASSETS_DIR "${CMAKE_CURRENT_LIST_DIR}/Assets")


if(NOT EMSCRIPTEN)
    add_subdirectory(EmptyWindow)
    add_subdirectory(HelloApp)
    add_subdirectory(EditorWithImGui)
    add_subdirectory(Example-Engine-MeshObject)
    add_subdirectory(ExternalOpenGLContext)
endif()
