
cmake_minimum_required(VERSION 3.0)
project(LuminoEngine)
include(cmake/LuminoConfig-LuminoEngine.cmake)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------
set(LN_SOURCES_Common
    include/LuminoEngine.hpp
    src/pch.hpp
    src/Internal.hpp
    src/RegisterTypes.cpp
)
file(GLOB LN_SOURCES_Runtime include/LuminoEngine/Runtime/*.hpp include/LuminoEngine/Runtime/detail/*.hpp src/Runtime/*.cpp src/Runtime/*.hpp)
file(GLOB LN_SOURCES_Base include/LuminoEngine/Base/*.hpp src/Base/*.cpp src/Base/*.hpp)
file(GLOB LN_SOURCES_Engine include/LuminoEngine/Engine/*.hpp src/Engine/*.cpp src/Engine/*.hpp)
file(GLOB LN_SOURCES_Asset include/LuminoEngine/Asset/*.hpp src/Asset/*.cpp src/Asset/*.hpp)
file(GLOB LN_SOURCES_Graphics include/LuminoEngine/Graphics/*.hpp src/Graphics/*.cpp src/Graphics/*.hpp)
file(GLOB LN_SOURCES_Platform include/LuminoEngine/*.hpp include/LuminoEngine/detail/*.hpp src/Platform/*.cpp src/Platform/*.hpp)
if (LN_EMSCRIPTEN)
elseif (WIN32)
    set(LN_SOURCES_Platform
        ${LN_SOURCES_Platform}
        include/LuminoEngine/Platform/Win32PlatformInterface.hpp
        src/Platform/Windows/Win32PlatformInterface.cpp
        src/Platform/Windows/Win32PlatformWindowManager.hpp
        src/Platform/Windows/Win32PlatformWindowManager.cpp
    )
elseif (APPLE)
    set(LN_SOURCES_Platform
        ${LN_SOURCES_Platform}
        src/Platform/iOS/iOSPlatformInterface.mm
        src/Platform/macOS/CocoaPlatformInterface.mm
        src/Platform/macOS/PlatformSupport.mm
    )
endif()



file(GLOB LOCAL_SOURCES
    "include/LuminoEngine/Graphics/GraphicsRHI/*.hpp"
    "include/LuminoEngine/Graphics/GraphicsRHI/detail/*.hpp"
    "src/Graphics/GraphicsRHI/*.hpp"
    "src/Graphics/GraphicsRHI/*.cpp"
)
file(GLOB LOCAL_SOURCES_ShaderCompiler
    "include/LuminoEngine/Graphics/GraphicsRHI/ShaderCompiler/detail/*.hpp"
    "src/Graphics/GraphicsRHI/ShaderCompiler/*.hpp"
    "src/Graphics/GraphicsRHI/ShaderCompiler/*.cpp"
)
file(GLOB LOCAL_SOURCES_ShaderCompiler_Lexer
    "src/Graphics/GraphicsRHI/ShaderCompiler/Lexer/*.hpp"
    "src/Graphics/GraphicsRHI/ShaderCompiler/Lexer/*.cpp"
)

if(LN_USE_OPENGL)
    file(GLOB LOCAL_SOURCES_OpenGL
        "src/Graphics/GraphicsRHI/OpenGL/*.hpp"
        "src/Graphics/GraphicsRHI/OpenGL/*.cpp"
    )
endif()

if(LN_USE_VULKAN)
    if(WIN32)
        file(GLOB LOCAL_SOURCES_Vulkan
            "include/LuminoEngine/Graphics/GraphicsRHI/Vulkan/*.hpp"
            "src/Graphics/GraphicsRHI/Vulkan/*.hpp"
            "src/Graphics/GraphicsRHI/Vulkan/*.cpp"
        )
    endif()
endif()

if(LN_EMSCRIPTEN)
elseif(WIN32)
    file(GLOB LOCAL_SOURCES_DirectX12
        "src/Graphics/GraphicsRHI/DirectX12/*.hpp"
        "src/Graphics/GraphicsRHI/DirectX12/*.cpp"
    )
elseif(APPLE)
endif()

if(LUMINO_USE_WEBGPU)
    file(GLOB LOCAL_SOURCES_WebGPU
        "include/LuminoEngine/Graphics/GraphicsRHI/WebGPU/*.hpp"
        "src/Graphics/GraphicsRHI/WebGPU/*.hpp"
        "src/Graphics/GraphicsRHI/WebGPU/*.cpp"
        "src/Graphics/GraphicsRHI/WebGPU/*.md"
    )
endif()

if(WIN32)
    file(GLOB LOCAL_SOURCES_ShaderCompiler_Windows
        "src/Graphics/GraphicsRHI/ShaderCompiler/Windows/*.hpp"
        "src/Graphics/GraphicsRHI/ShaderCompiler/Windows/*.cpp"
    )
endif()

file(GLOB LN_SOURCES_Audio "include/LuminoEngine/Audio/*.hpp" "src/Audio/*.cpp" "src/Audio/*.hpp" "src/Audio/*.md")
file(GLOB LN_SOURCES_Audio_Decoder "src/Audio/Decoder/*.cpp" "src/Audio/Decoder/*.hpp")
file(GLOB LN_SOURCES_Audio_ARIs "src/Audio/ARIs/*.cpp" "src/Audio/ARIs/*.hpp" "src/Audio/ARIs/*.h")
file(GLOB LN_SOURCES_Audio_Backend "src/Audio/Backend/*.cpp" "src/Audio/Backend/*.hpp")
file(GLOB LN_SOURCES_Animation "include/LuminoEngine/Animation/*.hpp" "src/Animation/*.cpp" "src/Animation/*.hpp")
file(GLOB LN_SOURCES_Graphics_Bitmap "include/LuminoEngine/Graphics/Bitmap/*.hpp" "include/LuminoEngine/Graphics/Bitmap/detail/*.hpp" "src/Graphics/Bitmap/*.cpp" "src/Graphics/Bitmap/*.hpp")
file(GLOB LN_SOURCES_Graphics_Font "include/LuminoEngine/Graphics/Font/*.hpp" "include/LuminoEngine/Graphics/Font/detail/*.hpp" "src/Graphics/Font/*.cpp" "src/Graphics/Font/*.hpp")
file(GLOB LN_SOURCES_Graphics_GPU "include/LuminoEngine/Graphics/GPU/*.hpp" "include/LuminoEngine/Graphics/GPU/detail/*.hpp" "src/Graphics/GPU/*.cpp" "src/Graphics/GPU/*.hpp" "src/Graphics/GPU/detail/*.cpp")
file(GLOB LN_SOURCES_Mesh "include/LuminoEngine/Mesh/*.hpp" "src/Mesh/*.cpp" "src/Mesh/*.hpp")
file(GLOB LN_SOURCES_Mesh_MeshImporters "include/LuminoEngine/Mesh/MeshGeneraters/*.hpp" "src/Mesh/MeshImporters/*.cpp" "src/Mesh/MeshImporters/*.hpp")
file(GLOB LN_SOURCES_Mesh_MeshGeneraters "include/LuminoEngine/Mesh/detail/*.hpp" "src/Mesh/MeshGeneraters/*.cpp" "src/Mesh/MeshGeneraters/*.hpp")
file(GLOB LN_SOURCES_Rendering "include/LuminoEngine/Rendering/*.hpp" "include/LuminoEngine/Rendering/detail/*.hpp" "src/Rendering/*.cpp" "src/Rendering/*.hpp")
file(GLOB LN_SOURCES_Rendering_RLIs "src/Rendering/RLIs/*.cpp" "src/Rendering/RLIs/*.hpp")
file(GLOB LN_SOURCES_Rendering_RenderFeature "include/LuminoEngine/Rendering/RenderFeature/*.hpp" "src/Rendering/RenderFeature/*.cpp" "src/Rendering/RenderFeature/*.hpp")
file(GLOB LN_SOURCES_Rendering_FeatureRenderer "include/LuminoEngine/Rendering/FeatureRenderer/*.hpp" "src/Rendering/FeatureRenderer/*.cpp" "src/Rendering/FeatureRenderer/*.hpp")
file(GLOB LN_SOURCES_Rendering_RenderingPipeline "include/LuminoEngine/Rendering/RenderingPipeline/*.hpp" "src/Rendering/RenderingPipeline/*.cpp" "src/Rendering/RenderingPipeline/*.hpp")
file(GLOB LN_SOURCES_Rendering_Kanata "include/LuminoEngine/Rendering/Kanata/*.hpp" "src/Rendering/Kanata/*.cpp" "src/Rendering/Kanata/*.hpp")
file(GLOB LN_SOURCES_Rendering_Kanata_RenderFeature "include/LuminoEngine/Rendering/Kanata/RenderFeature/*.hpp" "src/Rendering/Kanata/RenderFeature/*.cpp" "src/Rendering/Kanata/RenderFeature/*.hpp")
file(GLOB LN_SOURCES_Testing "include/LuminoEngine/Testing/*.hpp" "src/Testing/*.cpp" "src/Testing/*.hpp")
file(GLOB LN_SOURCES_Graphics_TextRendering
    "include/LuminoEngine/Graphics/TextRendering/*.hpp"
    "src/Graphics/TextRendering/*.cpp"
    "src/Graphics/TextRendering/*.hpp"
)
if (LN_USE_OPENGL)
    file(GLOB LN_SOURCES_GPU_RHI_OpenGL "src/GPU/RHI/OpenGL/*.cpp" "src/GPU/RHI/OpenGL/*.hpp")
endif()
if (LN_USE_VULKAN)
    if (WIN32)
        file(GLOB LN_SOURCES_GPU_RHI_Vulkan "src/GPU/RHI/Vulkan/*.cpp" "src/GPU/RHI/Vulkan/*.hpp")
    endif()
endif()
if (LN_EMSCRIPTEN)
elseif (WIN32)
    file(GLOB LN_SOURCES_GPU_RHI_DirectX12 "src/GPU/RHI/DirectX12/*.cpp" "src/GPU/RHI/DirectX12/*.hpp")
elseif (APPLE)
endif()

# Grouping.
source_group("" FILES ${LN_SOURCES_Common})
source_group("Runtime" FILES ${LN_SOURCES_Runtime})
source_group("Base" FILES ${LN_SOURCES_Base})
source_group("Engine" FILES ${LN_SOURCES_Engine})
source_group("Asset" FILES ${LN_SOURCES_Asset})
source_group("Audio" FILES ${LN_SOURCES_Audio})
source_group("Audio/Decoder" FILES ${LN_SOURCES_Audio_Decoder})
source_group("Audio/Backend" FILES ${LN_SOURCES_Audio_Backend})
source_group("Audio/ARIs" FILES ${LN_SOURCES_Audio_ARIs})

source_group("Platform" FILES ${LN_SOURCES_Platform})

source_group("Graphics" FILES ${LN_SOURCES_Graphics})
source_group("Graphics/Bitmap" FILES ${LN_SOURCES_Graphics_Bitmap})
source_group("Graphics/Font" FILES ${LN_SOURCES_Graphics_Font})
source_group("Graphics/GraphicsRHI" FILES ${LOCAL_SOURCES})
source_group("Graphics/GraphicsRHI/OpenGL" FILES ${LOCAL_SOURCES_OpenGL})
source_group("Graphics/GraphicsRHI/Vulkan" FILES ${LOCAL_SOURCES_Vulkan})
source_group("Graphics/GraphicsRHI/DirectX12" FILES ${LOCAL_SOURCES_DirectX12})
source_group("Graphics/GraphicsRHI/WebGPU" FILES ${LOCAL_SOURCES_WebGPU})
source_group("Graphics/GraphicsRHI/ShaderCompiler" FILES ${LOCAL_SOURCES_ShaderCompiler})
source_group("Graphics/GraphicsRHI/ShaderCompiler/Lexer" FILES ${LOCAL_SOURCES_ShaderCompiler_Lexer})
source_group("Graphics/GraphicsRHI/ShaderCompiler/Windows" FILES ${LOCAL_SOURCES_ShaderCompiler_Windows})
source_group("Graphics/GPU" FILES ${LN_SOURCES_Graphics_GPU})
source_group("Graphics/TextRendering" FILES ${LN_SOURCES_Graphics_TextRendering})

source_group("Animation" FILES ${LN_SOURCES_Animation})
source_group("Mesh" FILES ${LN_SOURCES_Mesh})
source_group("Mesh/MeshImporters" FILES ${LN_SOURCES_Mesh_MeshImporters})
source_group("Mesh/MeshGeneraters" FILES ${LN_SOURCES_Mesh_MeshGeneraters})
source_group("Rendering" FILES ${LN_SOURCES_Rendering})
source_group("Rendering/RLIs" FILES ${LN_SOURCES_Rendering_RLIs})
source_group("Rendering/RenderFeature" FILES ${LN_SOURCES_Rendering_RenderFeature})
source_group("Rendering/FeatureRenderer" FILES ${LN_SOURCES_Rendering_FeatureRenderer})
source_group("Rendering/RenderingPipeline" FILES ${LN_SOURCES_Rendering_RenderingPipeline})
source_group("Rendering/Kanata" FILES ${LN_SOURCES_Rendering_Kanata})
source_group("Rendering/Kanata/RenderFeature" FILES ${LN_SOURCES_Rendering_Kanata_RenderFeature})
source_group("Testing" FILES ${LN_SOURCES_Testing})

set(LN_SOURCES
    ${LN_SOURCES_Common}
    ${LN_SOURCES_Runtime}
    ${LN_SOURCES_Base}
    ${LN_SOURCES_Engine}
    ${LN_SOURCES_Asset}
    ${LN_SOURCES_Audio}
    ${LN_SOURCES_Audio_Decoder}
    ${LN_SOURCES_Audio_Backend}
    ${LN_SOURCES_Audio_ARIs}
    ${LN_SOURCES_Graphics}
    ${LN_SOURCES_Platform}
    ${LOCAL_SOURCES}
    ${LOCAL_SOURCES_OpenGL}
    ${LOCAL_SOURCES_Vulkan}
    ${LOCAL_SOURCES_DirectX12}
    ${LOCAL_SOURCES_WebGPU}
    ${LOCAL_SOURCES_ShaderCompiler}
    ${LOCAL_SOURCES_ShaderCompiler_Lexer}
    ${LOCAL_SOURCES_ShaderCompiler_Windows}
    ${LN_SOURCES_Graphics_Bitmap}
    ${LN_SOURCES_Animation}
	${LN_SOURCES_Graphics_Font}
    ${LN_SOURCES_Graphics_GPU}
    ${LN_SOURCES_GPU_RHI}
    ${LN_SOURCES_GPU_RHI_OpenGL}
    ${LN_SOURCES_GPU_RHI_Vulkan}
    ${LN_SOURCES_GPU_RHI_DirectX12}
    ${LN_SOURCES_Mesh}
    ${LN_SOURCES_Mesh_MeshImporters}
    ${LN_SOURCES_Mesh_MeshGeneraters}
    ${LN_SOURCES_Rendering}
    ${LN_SOURCES_Rendering_RLIs}
    ${LN_SOURCES_Rendering_RenderFeature}
    ${LN_SOURCES_Rendering_FeatureRenderer}
    ${LN_SOURCES_Rendering_RenderingPipeline}
    ${LN_SOURCES_Rendering_Kanata}
    ${LN_SOURCES_Rendering_Kanata_RenderFeature}
    ${LN_SOURCES_Testing}
    ${LN_SOURCES_Graphics_TextRendering}
)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------
set(LINK_LIBS
    LuminoCore
)

#if (NOT DEFINED EMSCRIPTEN)
#    list(APPEND LINK_LIBS CURL::libcurl)
#endif()

set(LOCAL_LIBRARIES 
    LuminoCore
    Vorbis::vorbis
    Vorbis::vorbisenc
    Vorbis::vorbisfile
    nanovg::nanovg
    ZLIB::ZLIB
    freetype
    tinyobjloader::tinyobjloader
)

# libpng is named differently depending on configuration.
if (LN_LINK_CMAKE_SHARED_LIB_PACKAGE)
    list(APPEND LOCAL_LIBRARIES png)
else()
    list(APPEND LOCAL_LIBRARIES png_static)
endif()

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
add_library(lumino::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(
    ${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(
    ${PROJECT_NAME} PRIVATE
    include
    src
    "../LuminoFFI/include"
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LINK_LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
target_precompile_headers(${PROJECT_NAME} PRIVATE "src/pch.hpp")

target_link_libraries(
    ${PROJECT_NAME} 
    PRIVATE
        ${LOCAL_LIBRARIES}
)

if (LN_OS_DESKTOP)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad::glad)
endif()
if (LN_EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-pthread")
endif()


if(LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER)
    target_compile_definitions(${PROJECT_NAME} PUBLIC LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER=1)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        glslang::OSDependent
        glslang::glslang
        glslang::MachineIndependent
        glslang::GenericCodeGen
        glslang::OGLCompiler
        glslang::glslang-default-resource-limits
        glslang::SPVRemapper
        glslang::SPIRV
        glslang::HLSL
        spirv-cross-core
        spirv-cross-glsl
        spirv-cross-hlsl
        spirv-cross-msl
        spirv-cross-reflect
        spirv-cross-util
    )
endif()

if(LN_USE_OPENGL)
    target_compile_definitions(${PROJECT_NAME} PUBLIC LN_USE_OPENGL=1)
endif()
if(LN_USE_VULKAN)
    target_compile_definitions(${PROJECT_NAME} PUBLIC LN_USE_VULKAN=1)
endif()
if(LN_USE_DIRECTX12)
    target_compile_definitions(${PROJECT_NAME} PUBLIC LN_USE_DIRECTX12=1)
endif()
if(LUMINO_USE_WEBGPU)
    target_link_libraries(${PROJECT_NAME} PRIVATE webgpu)
    target_compile_definitions(${PROJECT_NAME} PUBLIC LN_USE_WEBGPU=1)
endif()
if (LN_USE_OPENAL)
    target_compile_definitions(${PROJECT_NAME} PUBLIC LN_USE_OPENAL=1)
endif()

#-------------------------------------------------------------------------------
# Installation
#-------------------------------------------------------------------------------
if (LUMINO_INSTALL)
    install(
        TARGETS     ${PROJECT_NAME}
        EXPORT      LuminoExport
    )

    install(DIRECTORY include DESTINATION .)

    # install(EXPORT) で出力される LuminoCoreConfig.cmake は、同一フォルダの
    # LuminoCoreConfig-*.cmake を include しようとするため、それにマッチするファイル名にする。
    install(FILES cmake/LuminoConfig-${PROJECT_NAME}.cmake DESTINATION ${LN_EXPORT_DESTINATION_DIR})
endif()

#-------------------------------------------------------------------------------
# Sub-projects
#-------------------------------------------------------------------------------

if (LN_BUILD_TESTS)
    add_subdirectory(test)
    add_subdirectory(sandbox)
endif()

if(LUMINO_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


