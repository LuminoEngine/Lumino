project(LuminoGraphics)
include(cmake/LuminoConfig-LuminoGraphics.cmake)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------
file(GLOB LN_SOURCES "include/LuminoGraphics/*.hpp" "include/LuminoGraphics/detail/*.hpp" "include/*.hpp" "src/*.cpp" "src/*.hpp")

file(GLOB LN_SOURCES_Font "include/Font/*.hpp" "include/Font/detail/*.hpp" "src/Font/*.cpp" "src/Font/*.hpp")

file(GLOB LN_SOURCES_GPU "include/LuminoGraphics/GPU/*.hpp" "include/LuminoGraphics/GPU/detail/*.hpp" "src/GPU/*.cpp" "src/GPU/*.hpp")
file(GLOB LN_SOURCES_GPU_RHI "src/GPU/RHI/*.cpp" "src/GPU/RHI/*.hpp")

file(GLOB LN_SOURCES_ShaderCompiler "include/LuminoGraphics/ShaderCompiler/*.hpp" "include/LuminoGraphics/ShaderCompiler/detail/*.hpp" "src/ShaderCompiler/*.cpp" "src/ShaderCompiler/*.hpp")
file(GLOB LN_SOURCES_ShaderCompiler_Lexer "src/ShaderCompiler/Lexer/*.cpp" "src/ShaderCompiler/Lexer/*.hpp")

file(GLOB LN_SOURCES_Animation "include/LuminoGraphics/Animation/*.hpp" "src/Animation/*.cpp" "src/Animation/*.hpp")

file(GLOB LN_SOURCES_Mesh "include/LuminoGraphics/Mesh/*.hpp" "src/Mesh/*.cpp" "src/Mesh/*.hpp")
file(GLOB LN_SOURCES_Mesh_MeshImporters "include/LuminoGraphics/Mesh/MeshGeneraters/*.hpp" "src/Mesh/MeshImporters/*.cpp" "src/Mesh/MeshImporters/*.hpp")
file(GLOB LN_SOURCES_Mesh_MeshGeneraters "include/LuminoGraphics/Mesh/MeshGeneraters/*.hpp" "src/Mesh/MeshGeneraters/*.cpp" "src/Mesh/MeshGeneraters/*.hpp")

file(GLOB LN_SOURCES_TextRendering "include/LuminoGraphics/TextRendering/*.hpp" "src/TextRendering/*.cpp" "src/TextRendering/*.hpp")

file(GLOB LN_SOURCES_Testing "include/LuminoGraphics/Testing/*.hpp" "src/Testing/*.cpp" "src/Testing/*.hpp")

file(GLOB LN_SOURCES_Rendering "include/LuminoGraphics/Rendering/*.hpp" "include/LuminoGraphics/Rendering/detail/*.hpp" "src/Rendering/*.cpp" "src/Rendering/*.hpp")
file(GLOB LN_SOURCES_Rendering_RLIs "src/Rendering/RLIs/*.cpp" "src/Rendering/RLIs/*.hpp")
file(GLOB LN_SOURCES_Rendering_RenderFeature "include/LuminoGraphics/Rendering/RenderFeature/*.hpp" "src/Rendering/RenderFeature/*.cpp" "src/Rendering/RenderFeature/*.hpp")
file(GLOB LN_SOURCES_Rendering_FeatureRenderer "include/LuminoGraphics/Rendering/FeatureRenderer/*.hpp" "src/Rendering/FeatureRenderer/*.cpp" "src/Rendering/FeatureRenderer/*.hpp")
file(GLOB LN_SOURCES_Rendering_RenderingPipeline "include/LuminoGraphics/Rendering/RenderingPipeline/*.hpp" "src/Rendering/RenderingPipeline/*.cpp" "src/Rendering/RenderingPipeline/*.hpp")
file(GLOB LN_SOURCES_Rendering_Kanata "include/LuminoGraphics/Rendering/Kanata/*.hpp" "src/Rendering/Kanata/*.cpp" "src/Rendering/Kanata/*.hpp")
file(GLOB LN_SOURCES_Rendering_Kanata_RenderFeature "include/LuminoGraphics/Rendering/Kanata/RenderFeature/*.hpp" "src/Rendering/Kanata/RenderFeature/*.cpp" "src/Rendering/Kanata/RenderFeature/*.hpp")

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

if (WIN32)
    file(GLOB LN_SOURCES_ShaderCompiler_Windows "src/ShaderCompiler/Windows/*.cpp" "src/ShaderCompiler/Windows/*.hpp")
endif()

source_group("" FILES ${LN_SOURCES})
source_group("Font" FILES ${LN_SOURCES_Font})
source_group("GPU" FILES ${LN_SOURCES_GPU})
source_group("GPU/RHI" FILES ${LN_SOURCES_GPU_RHI})
source_group("GPU/RHI/OpenGL" FILES ${LN_SOURCES_GPU_RHI_OpenGL})
source_group("GPU/RHI/Vulkan" FILES ${LN_SOURCES_GPU_RHI_Vulkan})
source_group("GPU/RHI/DirectX12" FILES ${LN_SOURCES_GPU_RHI_DirectX12})
source_group("ShaderCompiler" FILES ${LN_SOURCES_ShaderCompiler})
source_group("ShaderCompiler/Lexer" FILES ${LN_SOURCES_ShaderCompiler_Lexer})
source_group("ShaderCompiler/Windows" FILES ${LN_SOURCES_ShaderCompiler_Windows})
source_group("Animation" FILES ${LN_SOURCES_Animation})
source_group("Mesh" FILES ${LN_SOURCES_Mesh})
source_group("Mesh/MeshImporters" FILES ${LN_SOURCES_Mesh_MeshImporters})
source_group("Mesh/MeshGeneraters" FILES ${LN_SOURCES_Mesh_MeshGeneraters})
source_group("TextRendering" FILES ${LN_SOURCES_TextRendering})
source_group("Testing" FILES ${LN_SOURCES_Testing})
source_group("Rendering" FILES ${LN_SOURCES_Rendering})
source_group("Rendering/RLIs" FILES ${LN_SOURCES_Rendering_RLIs})
source_group("Rendering/RenderFeature" FILES ${LN_SOURCES_Rendering_RenderFeature})
source_group("Rendering/FeatureRenderer" FILES ${LN_SOURCES_Rendering_FeatureRenderer})
source_group("Rendering/RenderingPipeline" FILES ${LN_SOURCES_Rendering_RenderingPipeline})
source_group("Rendering/Kanata" FILES ${LN_SOURCES_Rendering_Kanata})
source_group("Rendering/Kanata/RenderFeature" FILES ${LN_SOURCES_Rendering_Kanata_RenderFeature})

set(LN_SOURCES
    ${LN_SOURCES}
	${LN_SOURCES_Font}
    ${LN_SOURCES_GPU}
    ${LN_SOURCES_GPU_RHI}
    ${LN_SOURCES_GPU_RHI_OpenGL}
    ${LN_SOURCES_GPU_RHI_Vulkan}
    ${LN_SOURCES_GPU_RHI_DirectX12}
    ${LN_SOURCES_ShaderCompiler}
    ${LN_SOURCES_ShaderCompiler_Lexer}
    ${LN_SOURCES_ShaderCompiler_Windows}
    ${LN_SOURCES_Animation}
    ${LN_SOURCES_Mesh}
    ${LN_SOURCES_Mesh_MeshImporters}
    ${LN_SOURCES_Mesh_MeshGeneraters}
    ${LN_SOURCES_TextRendering}
    ${LN_SOURCES_Testing}
    ${LN_SOURCES_Rendering}
    ${LN_SOURCES_Rendering_RLIs}
    ${LN_SOURCES_Rendering_RenderFeature}
    ${LN_SOURCES_Rendering_FeatureRenderer}
    ${LN_SOURCES_Rendering_RenderingPipeline}
    ${LN_SOURCES_Rendering_Kanata}
    ${LN_SOURCES_Rendering_Kanata_RenderFeature}
)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------
if (LN_USE_VULKAN)
    list(APPEND LN_DEFINITIONS LN_USE_VULKAN=1)
endif()

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
target_include_directories(
    ${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(${PROJECT_NAME} PRIVATE include src) # for PCH
target_link_libraries(${PROJECT_NAME} PRIVATE
    LuminoCore
    LuminoRuntime
    LuminoPlatform
    LuminoBitmap
	freetype
    tinyobjloader::tinyobjloader
)
target_compile_definitions(${PROJECT_NAME} PRIVATE ${LN_DEFINITIONS})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/pch.hpp" "src/pch.cpp")

if (LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER)
    target_compile_definitions(${PROJECT_NAME} PRIVATE LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER=1)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        HLSL
        SPIRV
        glslang
        OGLCompiler
        spirv-cross-core
        spirv-cross-glsl
        spirv-cross-hlsl
        spirv-cross-msl
        spirv-cross-reflect
        spirv-cross-util
    )
endif()

#-------------------------------------------------------------------------------
# Installation
#-------------------------------------------------------------------------------
install(
    TARGETS     ${PROJECT_NAME}
    EXPORT      LuminoExport
    ARCHIVE     DESTINATION ./lib
    INCLUDES    DESTINATION ./include
)
install(DIRECTORY include DESTINATION .)
install(FILES cmake/LuminoConfig-${PROJECT_NAME}.cmake DESTINATION lib/cmake/Lumino)

#-------------------------------------------------------------------------------
# Sub-projects
#-------------------------------------------------------------------------------

if (LN_BUILD_TESTS)
    add_subdirectory(test)
    add_subdirectory(sandbox)
endif()

if (LN_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
