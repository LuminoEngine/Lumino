project(LuminoGraphicsRHI)
include(cmake/LuminoConfig-LuminoGraphicsRHI.cmake)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------
file(GLOB LOCAL_SOURCES
    "include/LuminoGraphicsRHI/*.hpp"
    "include/LuminoGraphicsRHI/detail/*.hpp"
    "include/*.hpp"
    "src/*.hpp"
    "src/*.cpp")
file(GLOB LOCAL_SOURCES_ShaderCompiler "include/LuminoGraphicsRHI/ShaderCompiler/detail/*.hpp" "src/ShaderCompiler/*.hpp" "src/ShaderCompiler/*.cpp")
file(GLOB LOCAL_SOURCES_ShaderCompiler_Lexer "src/ShaderCompiler/Lexer/*.hpp" "src/ShaderCompiler/Lexer/*.cpp")

if(LN_USE_OPENGL)
    file(GLOB LOCAL_SOURCES_OpenGL "src/OpenGL/*.hpp" "src/OpenGL/*.cpp")
endif()

if(LN_USE_VULKAN)
    if(WIN32)
        file(GLOB LOCAL_SOURCES_Vulkan "include/LuminoGraphicsRHI/Vulkan/*.hpp" "src/Vulkan/*.hpp" "src/Vulkan/*.cpp")
    endif()
endif()

if(LN_EMSCRIPTEN)
elseif(WIN32)
    file(GLOB LOCAL_SOURCES_DirectX12 "src/DirectX12/*.hpp" "src/DirectX12/*.cpp")
elseif(APPLE)
endif()

if(LUMINO_USE_WEBGPU)
    file(GLOB LOCAL_SOURCES_WebGPU "include/LuminoGraphicsRHI/WebGPU/*.hpp" "src/WebGPU/*.hpp" "src/WebGPU/*.cpp" "src/WebGPU/*.md")
endif()

if(WIN32)
    file(GLOB LOCAL_SOURCES_ShaderCompiler_Windows "src/ShaderCompiler/Windows/*.hpp" "src/ShaderCompiler/Windows/*.cpp")
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
    target_compile_definitions(${PROJECT_NAME} PUBLIC LUMINO_USE_WEBGPU=1)
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
