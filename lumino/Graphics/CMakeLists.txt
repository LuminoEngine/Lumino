project(LuminoGraphics)
include(cmake/LuminoConfig-LuminoGraphics.cmake)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------
file(GLOB LN_SOURCES "include/LuminoGraphics/*.hpp" "include/*.hpp" "src/*.cpp" "src/*.hpp")

file(GLOB LN_SOURCES_RHI "include/LuminoGraphics/RHI/*.hpp" "include/LuminoGraphics/RHI/detail/*.hpp" "src/RHI/*.cpp" "src/RHI/*.hpp")
file(GLOB LN_SOURCES_RHI_Backend "src/RHI/Backend/*.cpp" "src/RHI/Backend/*.hpp")

file(GLOB LN_SOURCES_ShaderCompiler "include/LuminoGraphics/ShaderCompiler/*.hpp" "include/LuminoGraphics/ShaderCompiler/detail/*.hpp" "src/ShaderCompiler/*.cpp" "src/ShaderCompiler/*.hpp")
file(GLOB LN_SOURCES_ShaderCompiler_Lexer "src/ShaderCompiler/Lexer/*.cpp" "src/ShaderCompiler/Lexer/*.hpp")

file(GLOB LN_SOURCES_Rendering "include/LuminoGraphics/Rendering/*.hpp" "src/Rendering/*.cpp" "src/Rendering/*.hpp")

if (LN_USE_OPENGL)
    file(GLOB LN_SOURCES_RHI_Backend_OpenGL "src/RHI/Backend/OpenGL/*.cpp" "src/RHI/Backend/OpenGL/*.hpp")
endif()

if (LN_USE_VULKAN)
    if (WIN32)
        file(GLOB LN_SOURCES_RHI_Backend_Vulkan "src/RHI/Backend/Vulkan/*.cpp" "src/RHI/Backend/Vulkan/*.hpp")
    endif()
endif()

if (LN_EMSCRIPTEN)
elseif (WIN32)
    file(GLOB LN_SOURCES_RHI_Backend_DirectX12 "src/RHI/Backend/DirectX12/*.cpp" "src/RHI/Backend/DirectX12/*.hpp")
elseif (APPLE)
endif()

if (WIN32)
    file(GLOB LN_SOURCES_ShaderCompiler_Windows "src/ShaderCompiler/Windows/*.cpp" "src/ShaderCompiler/Windows/*.hpp")
endif()

source_group("" FILES ${LN_SOURCES})
source_group("RHI" FILES ${LN_SOURCES_RHI})
source_group("RHI/Backend" FILES ${LN_SOURCES_RHI_Backend})
source_group("RHI/Backend/OpenGL" FILES ${LN_SOURCES_RHI_Backend_OpenGL})
source_group("RHI/Backend/Vulkan" FILES ${LN_SOURCES_RHI_Backend_Vulkan})
source_group("RHI/Backend/DirectX12" FILES ${LN_SOURCES_RHI_Backend_DirectX12})
source_group("ShaderCompiler" FILES ${LN_SOURCES_ShaderCompiler})
source_group("ShaderCompiler/Lexer" FILES ${LN_SOURCES_ShaderCompiler_Lexer})
source_group("ShaderCompiler/Windows" FILES ${LN_SOURCES_ShaderCompiler_Windows})
source_group("Rendering" FILES ${LN_SOURCES_Rendering})

set(LN_SOURCES
    ${LN_SOURCES}
    ${LN_SOURCES_RHI}
    ${LN_SOURCES_RHI_Backend}
    ${LN_SOURCES_RHI_Backend_OpenGL}
    ${LN_SOURCES_RHI_Backend_Vulkan}
    ${LN_SOURCES_RHI_Backend_DirectX12}
    ${LN_SOURCES_ShaderCompiler}
    ${LN_SOURCES_ShaderCompiler_Lexer}
    ${LN_SOURCES_ShaderCompiler_Windows}
    ${LN_SOURCES_Rendering}
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
    LuminoFont
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
