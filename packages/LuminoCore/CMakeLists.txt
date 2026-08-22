project(LuminoCore)

#-------------------------------------------------------------------------------
# Dependencies
#-------------------------------------------------------------------------------
if(LN_ENABLE_VULKAN)
    # Vulkan ローダーは volk 経由で実行時に動的ロードするため、ビルドに必要なのは
    # ヘッダだけです。Vulkan SDK のインストールや vulkan-1.lib のリンクは不要です。
    find_package(VulkanHeaders CONFIG REQUIRED)
    find_package(volk CONFIG REQUIRED)
endif()

find_package(Stb REQUIRED)

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

if(LN_ENABLE_VULKAN)
    file(GLOB LN_LOCAL_SOURCES_Graphics_vulkan
        src/Graphics/rhi/vulkan/*.cpp
        src/Graphics/rhi/vulkan/*.hpp
    )
endif()

if(LN_ENABLE_WEBGPU)
    file(GLOB LN_LOCAL_SOURCES_Graphics_webgpu
        src/Graphics/rhi/webgpu/*.cpp
        src/Graphics/rhi/webgpu/*.hpp
    )
endif()

file(GLOB LN_LOCAL_SOURCES_shaders shaders/*.slang)

# Emscripten: exclude desktop-only Graphics files
if(EMSCRIPTEN)
    set(LN_DESKTOP_ONLY_GRAPHICS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/DebugPrint.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/ForwardRenderer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/MeshLoader.cpp
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
if(LN_ENABLE_VULKAN)
    source_group("Graphics/vulkan" FILES ${LN_LOCAL_SOURCES_Graphics_vulkan})
endif()
if(LN_ENABLE_WEBGPU)
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

target_link_libraries(${PROJECT_NAME} PRIVATE LuminoShader)
target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${Stb_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../LuminoC/include
)

# Vulkan
if(LN_ENABLE_VULKAN)
    # volk::volk_headers はヘッダ (volk.h / volk.c) のみを提供します。
    # 実装は VulkanLoader.cpp が VOLK_IMPLEMENTATION 付きで取り込むため、
    # volk.c は Lumino 本体とまったく同じコンパイル定義でビルドされます。
    # (プリビルドの volk::volk を使うと VK_USE_PLATFORM_* の差異で
    #  VolkDeviceTable のレイアウトがずれる危険があります)
    target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Headers volk::volk_headers)
    target_compile_definitions(${PROJECT_NAME} PRIVATE LN_ENABLE_VULKAN=1)
endif()

# GLFW (desktop only)
if(LN_PLATFORM_DESKTOP)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
endif()

# WebGPU
if(LN_ENABLE_WEBGPU)
    target_link_libraries(${PROJECT_NAME} PRIVATE webgpu)
    target_compile_definitions(${PROJECT_NAME} PRIVATE LN_ENABLE_WEBGPU=1)
endif()

#-------------------------------------------------------------------------------
# Shader compilation (slang → lcsh → lcsh.inl) - desktop only
#-------------------------------------------------------------------------------
if(LN_PLATFORM_DESKTOP)
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
