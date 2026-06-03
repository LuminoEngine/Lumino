
set(LOCAL_ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")

# Setup sample project
function(setup_sample_configurations TARGET)
    target_compile_definitions(${TARGET} PRIVATE ASSETS_DIR="${LOCAL_ASSETS_DIR}")
    set_target_properties(${TARGET} PROPERTIES FOLDER "examples")
    target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    setup_c_api_client_configurations(${TARGET})
endfunction()

#-------------------------------------------------------------------------------
# Examples
#-------------------------------------------------------------------------------
add_subdirectory(ClearScreen)
add_subdirectory(HelloTexture)
add_subdirectory(BatchSprite)
add_subdirectory(BatchRendering)
#add_subdirectory(Sprite2D)
add_subdirectory(CustomShader)
add_subdirectory(RenderTargets)
add_subdirectory(SSR)

