project(LuminoGraphicsRHI)
include(cmake/LuminoConfig-LuminoGraphicsRHI.cmake)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------
file(GLOB LOCAL_SOURCES
    "include/LuminoGraphicsRHI/*.hpp"
    "include/LuminoGraphicsRHI/detail/*.hpp"
    "include/*.hpp"
    "src/*.cpp"
    "src/*.hpp")
file(GLOB LOCAL_SOURCES_ShaderCompiler "include/LuminoGraphicsRHI/ShaderCompiler/detail/*.hpp" "src/ShaderCompiler/*.cpp" "src/ShaderCompiler/*.hpp")
file(GLOB LOCAL_SOURCES_ShaderCompiler_Lexer "src/ShaderCompiler/Lexer/*.cpp" "src/ShaderCompiler/Lexer/*.hpp")

if(LN_USE_OPENGL)
    file(GLOB LOCAL_SOURCES_OpenGL "src/OpenGL/*.cpp" "src/OpenGL/*.hpp")
endif()

if(LN_USE_VULKAN)
    if(WIN32)
        file(GLOB LOCAL_SOURCES_Vulkan "src/Vulkan/*.cpp" "src/Vulkan/*.hpp")
    endif()
endif()

if(LN_EMSCRIPTEN)
elseif(WIN32)
    file(GLOB LOCAL_SOURCES_DirectX12 "src/DirectX12/*.cpp" "src/DirectX12/*.hpp")
elseif(APPLE)
endif()

if(LUMINO_USE_WEBGPU)
    file(GLOB LOCAL_SOURCES_WebGPU "include/LuminoGraphicsRHI/WebGPU/*.hpp" "src/WebGPU/*.cpp" "src/WebGPU/*.hpp" "src/WebGPU/*.md")
endif()

if(WIN32)
    file(GLOB LOCAL_SOURCES_ShaderCompiler_Windows "src/ShaderCompiler/Windows/*.cpp" "src/ShaderCompiler/Windows/*.hpp")
endif()

source_group("Source Files" FILES ${LOCAL_SOURCES})
source_group("Source Files/OpenGL" FILES ${LOCAL_SOURCES_OpenGL})
source_group("Source Files/Vulkan" FILES ${LOCAL_SOURCES_Vulkan})
source_group("Source Files/DirectX12" FILES ${LOCAL_SOURCES_DirectX12})
source_group("Source Files/WebGPU" FILES ${LOCAL_SOURCES_WebGPU})
source_group("Source Files/ShaderCompiler" FILES ${LOCAL_SOURCES_ShaderCompiler})
source_group("Source Files/ShaderCompiler/Lexer" FILES ${LOCAL_SOURCES_ShaderCompiler_Lexer})
source_group("Source Files/ShaderCompiler/Windows" FILES ${LOCAL_SOURCES_ShaderCompiler_Windows})
set(LOCAL_SOURCES
    ${LOCAL_SOURCES}
    ${LOCAL_SOURCES_OpenGL}
    ${LOCAL_SOURCES_Vulkan}
    ${LOCAL_SOURCES_DirectX12}
    ${LOCAL_SOURCES_WebGPU}
    ${LOCAL_SOURCES_ShaderCompiler}
    ${LOCAL_SOURCES_ShaderCompiler_Lexer}
    ${LOCAL_SOURCES_ShaderCompiler_Windows}
)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------

set(LOCAL_LIBRARIES 
    LuminoCore
    LuminoRuntime
    LuminoPlatform
    Vulkan::Headers
)

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------

add_library(${PROJECT_NAME} STATIC ${LOCAL_SOURCES})
add_library(lumino::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(
    ${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(${PROJECT_NAME} PRIVATE include src) # for PCH
target_link_libraries(${PROJECT_NAME} PRIVATE ${LOCAL_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${LN_DEFINITIONS})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
target_precompile_headers(${PROJECT_NAME} PRIVATE "src/pch.hpp")

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

if(LN_USE_VULKAN)
    target_compile_definitions(${PROJECT_NAME} PUBLIC LN_USE_VULKAN=1)
endif()

if(LUMINO_USE_WEBGPU)
    target_link_libraries(${PROJECT_NAME} PRIVATE webgpu)
endif()

#-------------------------------------------------------------------------------
# Installation
#-------------------------------------------------------------------------------
if(LUMINO_INSTALL)
    install(
        TARGETS     ${PROJECT_NAME}
        EXPORT      LuminoExport
    )
    install(DIRECTORY include DESTINATION .)
    install(FILES cmake/LuminoConfig-${PROJECT_NAME}.cmake DESTINATION ${LN_EXPORT_DESTINATION_DIR})
endif()

#-------------------------------------------------------------------------------
# Sub-projects
#-------------------------------------------------------------------------------

if(LN_BUILD_TESTS)
    add_subdirectory(test)
endif()

if(LUMINO_BUILD_EXAMPLES)
    add_subdirectory(examples/GraphicsRHI-1-ClearScreen)
    add_subdirectory(examples/GraphicsRHI-HelloTriangle)
endif()
