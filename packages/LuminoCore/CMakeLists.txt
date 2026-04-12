project(LuminoCore)

#-------------------------------------------------------------------------------
# Dependencies
#-------------------------------------------------------------------------------
if(LUMINO_ENABLE_VULKAN)
    find_package(Vulkan QUIET)
    if(NOT Vulkan_FOUND)
        find_package(vulkan-headers CONFIG REQUIRED)
    endif()
endif()

if(NOT EMSCRIPTEN)
    find_package(Stb REQUIRED)
endif()

#-------------------------------------------------------------------------------
# Sources
#-------------------------------------------------------------------------------
file(GLOB LN_LOCAL_SOURCES_core
    src/pch.hpp
    src/CoreInstance.cpp
    src/Object.cpp
    src/ObjectRegistry.cpp
    include/${PROJECT_NAME}/CoreInstance.hpp
    include/${PROJECT_NAME}/Object.hpp
    include/${PROJECT_NAME}/ObjectRegistry.hpp
)
file(GLOB LN_LOCAL_SOURCES_Runtime src/Runtime/*.cpp src/Runtime/*.hpp include/${PROJECT_NAME}/Runtime/*.hpp)
file(GLOB LN_LOCAL_SOURCES_Platform src/Platform/*.cpp src/Platform/*.hpp include/${PROJECT_NAME}/Platform/*.hpp)
file(GLOB LN_LOCAL_SOURCES_Graphics
    src/Graphics/*.cpp
    src/Graphics/*.hpp
    include/${PROJECT_NAME}/Graphics/*.hpp
)

if(LUMINO_ENABLE_VULKAN)
    file(GLOB LN_LOCAL_SOURCES_Graphics_vulkan
        src/Graphics/rhi/vulkan/*.cpp
        src/Graphics/rhi/vulkan/*.hpp
    )
endif()

if(LUMINO_ENABLE_WEBGPU)
    file(GLOB LN_LOCAL_SOURCES_Graphics_webgpu
        src/Graphics/rhi/webgpu/*.cpp
        src/Graphics/rhi/webgpu/*.hpp
    )
endif()

file(GLOB LN_LOCAL_SOURCES_shaders shaders/*.slang)

# Emscripten: exclude desktop-only Graphics files (depend on Stb/LuminoShader/desktop features)
if(EMSCRIPTEN)
    set(LN_DESKTOP_ONLY_GRAPHICS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/Batch.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/Camera.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/DebugPrint.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/DynamicUniformAllocator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/ForwardRenderer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/GraphicsModule.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/Material.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/Mesh.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/MeshLoader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/PipelineCache.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/Renderer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/ShaderPass.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/TextureLoader.cpp
    )
    list(REMOVE_ITEM LN_LOCAL_SOURCES_Graphics ${LN_DESKTOP_ONLY_GRAPHICS})

    # DeviceFactory_Web.cpp lives in rhi/ (not covered by the Graphics glob)
    list(APPEND LN_LOCAL_SOURCES_Graphics
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/rhi/DeviceFactory_Web.cpp
    )
endif()

source_group("" FILES ${LN_LOCAL_SOURCES_core})
source_group("Runtime" FILES ${LN_LOCAL_SOURCES_Runtime})
source_group("Platform" FILES ${LN_LOCAL_SOURCES_Platform})
source_group("Graphics" FILES ${LN_LOCAL_SOURCES_Graphics})
if(LUMINO_ENABLE_VULKAN)
    source_group("Graphics/vulkan" FILES ${LN_LOCAL_SOURCES_Graphics_vulkan})
endif()
if(LUMINO_ENABLE_WEBGPU)
    source_group("Graphics/webgpu" FILES ${LN_LOCAL_SOURCES_Graphics_webgpu})
endif()
source_group("shaders" FILES ${LN_LOCAL_SOURCES_shaders})

set(LN_LOCAL_SOURCES
    ${LN_LOCAL_SOURCES_core}
    ${LN_LOCAL_SOURCES_Runtime}
    ${LN_LOCAL_SOURCES_Platform}
    ${LN_LOCAL_SOURCES_Graphics}
    ${LN_LOCAL_SOURCES_Graphics_vulkan}
    ${LN_LOCAL_SOURCES_Graphics_webgpu}
    ${LN_LOCAL_SOURCES_shaders}
)

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_library(
    ${PROJECT_NAME} STATIC
    ${LN_LOCAL_SOURCES}
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)
target_link_libraries(${PROJECT_NAME} PUBLIC LuminoBase)

# Desktop-only dependencies
if(NOT EMSCRIPTEN)
    target_link_libraries(${PROJECT_NAME} PRIVATE LuminoShader)
    target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
endif()

# Vulkan
if(LUMINO_ENABLE_VULKAN)
    if(Vulkan_FOUND)
        target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)
    else()
        target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Headers)
        # Link the Vulkan loader
        if(WIN32)
            target_link_libraries(${PROJECT_NAME} PRIVATE vulkan-1)
        else()
            target_link_libraries(${PROJECT_NAME} PRIVATE vulkan)
        endif()
    endif()
endif()

# GLFW (desktop only)
if(LUMINO_PLATFORM_DESKTOP)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
endif()

# WebGPU
if(LUMINO_ENABLE_WEBGPU)
    target_link_libraries(${PROJECT_NAME} PRIVATE webgpu)
    target_compile_definitions(${PROJECT_NAME} PRIVATE LUMINO_ENABLE_WEBGPU)
endif()

#-------------------------------------------------------------------------------
# Shader compilation (slang → lcsh → lcsh.inl) — desktop only
#-------------------------------------------------------------------------------
if(NOT LUMINO_PLATFORM_WEB)
    set(LN_SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)

    set(LN_BUILTIN_SHADERS
        Unlit
        BasicLit
        StencilMask
    )

    foreach(SHADER_NAME ${LN_BUILTIN_SHADERS})
        set(SHADER_SRC ${LN_SHADER_DIR}/${SHADER_NAME}.slang)
        set(SHADER_INL ${LN_SHADER_DIR}/${SHADER_NAME}.lcsh.inl)

        add_custom_command(
            OUTPUT ${SHADER_INL}
            COMMAND luminosc -I ${CMAKE_CURRENT_SOURCE_DIR}/../LuminoShader/shaders ${SHADER_SRC}
            DEPENDS ${SHADER_SRC} luminosc ${CMAKE_CURRENT_SOURCE_DIR}/../LuminoShader/shaders/lumino.slang
            COMMENT "Compiling shader ${SHADER_NAME}.slang"
        )
        list(APPEND LN_SHADER_INL_FILES ${SHADER_INL})
    endforeach()

    add_custom_target(LuminoCore_shaders DEPENDS ${LN_SHADER_INL_FILES})
    add_dependencies(${PROJECT_NAME} LuminoCore_shaders)

    # Make generated .inl files findable by Material.cpp via #include "shaders/..."
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

#-------------------------------------------------------------------------------
# Subdirectories
#-------------------------------------------------------------------------------
if(LUMINO_BUILD_TESTS)
    add_subdirectory(test)
endif()
if(LUMINO_BUILD_EXAMPLES)
    add_subdirectory(examples/HelloMesh)
    add_subdirectory(examples/HelloTriangle)
    add_subdirectory(examples/ForwardRendering)
    add_subdirectory(examples/HelloTexture)
endif()
