
set(LOCAL_ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")

# Setup sample project
function(setup_sample_configurations TARGET)
    target_compile_definitions(${TARGET} PRIVATE ASSETS_DIR="${LOCAL_ASSETS_DIR}")
    target_link_libraries(${TARGET} PRIVATE LuminoC glfw)
    set_target_properties(${TARGET} PROPERTIES FOLDER "examples")
    add_dependencies(${TARGET} LuminoC)
    target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    # WebGPU (copy dll)
    if(LN_ENABLE_WEBGPU)
        target_copy_webgpu_binaries(${TARGET})
    endif()

    if(LUMINO_USE_SLANG)
        target_link_libraries(${TARGET} PRIVATE slang::slang)
    endif()

    # copy dll
    add_custom_command(
        TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:LuminoC>
        $<TARGET_FILE_DIR:${TARGET}>
    )

    # DXC libraries required by Dawn's D3D12 backend
    if(LN_ENABLE_WEBGPU AND TARGET Microsoft::DirectXShaderCompiler)
        add_custom_command(
            TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:Microsoft::DirectXShaderCompiler>
                $<TARGET_FILE_DIR:${TARGET}>
        )
        add_custom_command(
            TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:Microsoft::DXIL>
                $<TARGET_FILE_DIR:${TARGET}>
        )
    endif()
endfunction()

#-------------------------------------------------------------------------------
# Examples
#-------------------------------------------------------------------------------
add_subdirectory(ClearScreen)
add_subdirectory(HelloTexture)
add_subdirectory(BatchSprite)
add_subdirectory(BatchRendering)
add_subdirectory(Sprite2D)
add_subdirectory(CustomShader)
add_subdirectory(RenderTargets)
add_subdirectory(SSR)

