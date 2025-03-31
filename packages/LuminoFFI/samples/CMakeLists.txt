
find_package(glfw3 CONFIG REQUIRED)

set(LOCAL_ASSETS_DIR "${CMAKE_CURRENT_LIST_DIR}/Assets")

# Setup LuminoFFI sample projects
function(target_set_sample_configurations Target)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ASSETS_DIR="${LOCAL_ASSETS_DIR}")
    target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore LuminoFFI-static glfw)
    set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "samples")

    if(LN_DESKTOP_WEBGPU)
        target_link_libraries(${PROJECT_NAME} PRIVATE webgpu)
        target_copy_webgpu_binaries(${PROJECT_NAME})
    endif()
    
    # Copy LuminoFFI.dll to output directory
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:LuminoFFI>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )

endfunction()

if(LN_OS_DESKTOP)
    add_subdirectory(LuminoFFI-OpenGLApp)
    add_subdirectory(LuminoFFI-1-ClearScreen)
    add_subdirectory(LuminoFFI-2-HelloSprite)
endif()
