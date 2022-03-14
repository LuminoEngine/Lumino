project(LuminoShaderCompiler)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

file(GLOB LN_SOURCES_ShaderCompiler include/LuminoShaderCompiler/*.hpp include/LuminoShaderCompiler/detail/*.hpp  src/*.cpp src/*.hpp)
file(GLOB LN_SOURCES_Lexer src/Lexer/*.cpp src/Lexer/*.hpp)


if (WIN32)
    file(GLOB LN_SOURCES_Windows src/Windows/*.cpp src/Windows/*.hpp)
endif()

source_group("ShaderCompiler" FILES ${LN_SOURCES_ShaderCompiler})
source_group("Lexer" FILES ${LN_SOURCES_Lexer})
source_group("Windows" FILES ${LN_SOURCES_Windows})

set(LN_SOURCES
    src/pch.cpp
    src/pch.hpp
    src/Internal.hpp
    ${LN_SOURCES_ShaderCompiler}
    ${LN_SOURCES_Lexer}
    ${LN_SOURCES_Windows}
)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------


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
)

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

# set(LN_TARGETS_NAME "${PROJECT_NAME}Targets")
# set(LN_CONFIG_NAME "${PROJECT_NAME}Config")

# install(
#     TARGETS ${PROJECT_NAME}
#     EXPORT ${LN_TARGETS_NAME}
#     ARCHIVE DESTINATION ./lib
#     INCLUDES DESTINATION ./include
# )

# install(DIRECTORY include DESTINATION .)

# install(
#     EXPORT ${LN_TARGETS_NAME}
#     FILE ${LN_CONFIG_NAME}.cmake
#     DESTINATION lib/cmake/${PROJECT_NAME}
#     EXPORT_LINK_INTERFACE_LIBRARIES
# )


install(
    TARGETS ${PROJECT_NAME}
    EXPORT LuminoEngine-Export
    ARCHIVE DESTINATION ./lib
    INCLUDES DESTINATION ./include
)

install(DIRECTORY include DESTINATION .)

#-------------------------------------------------------------------------------
# Sub-projects
#-------------------------------------------------------------------------------

if (LN_BUILD_TESTS AND LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER)
    add_subdirectory(test)
endif()
