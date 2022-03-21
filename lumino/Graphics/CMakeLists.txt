project(LuminoGraphics)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

file(GLOB LN_SOURCES_Graphics include/LuminoGraphics/*.hpp include/LuminoGraphics/detail/*.hpp src/*.cpp src/*.hpp)
file(GLOB LN_SOURCES_Graphics_RHIs include/LuminoGraphics/RHIs/*.hpp src/RHIs/*.cpp src/RHIs/*.hpp)

if (LN_USE_OPENGL)
    file(GLOB LN_SOURCES_Graphics_RHIs_OpenGL src/RHIs/OpenGL/*.cpp src/RHIs/OpenGL/*.hpp)
endif()

if (LN_USE_VULKAN)
    if (WIN32)
        file(GLOB LN_SOURCES_Graphics_RHIs_Vulkan src/RHIs/Vulkan/*.cpp src/RHIs/Vulkan/*.hpp)
    endif()
endif()

if (LN_EMSCRIPTEN)
elseif (WIN32)
    file(GLOB LN_SOURCES_Graphics_RHIs_DirectX12 src/RHIs/DirectX12/*.cpp src/RHIs/DirectX12/*.hpp)
elseif (APPLE)
endif()

source_group("Graphics" FILES ${LN_SOURCES_Graphics})
source_group("Graphics\\RHIs" FILES ${LN_SOURCES_Graphics_RHIs})
source_group("Graphics\\RHIs\\OpenGL" FILES ${LN_SOURCES_Graphics_RHIs_OpenGL})
source_group("Graphics\\RHIs\\Vulkan" FILES ${LN_SOURCES_Graphics_RHIs_Vulkan})
source_group("Graphics\\RHIs\\DirectX12" FILES ${LN_SOURCES_Graphics_RHIs_DirectX12})

set(LN_SOURCES
    src/pch.cpp
    src/pch.hpp
    src/Internal.hpp
    ${LN_SOURCES_Graphics}
    ${LN_SOURCES_Graphics_RHIs}
    ${LN_SOURCES_Graphics_RHIs_OpenGL}
    ${LN_SOURCES_Graphics_RHIs_Vulkan}
    ${LN_SOURCES_Graphics_RHIs_DirectX12}
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
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore LuminoRuntime LuminoPlatform LuminoShaderCompiler LuminoBitmap LuminoFont)
target_compile_definitions(${PROJECT_NAME} PRIVATE ${LN_DEFINITIONS})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/pch.hpp" "src/pch.cpp")

if (LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER)
    target_compile_definitions(${PROJECT_NAME} PRIVATE LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER=1)
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
    EXPORT LuminoExport
    ARCHIVE DESTINATION ./lib
    INCLUDES DESTINATION ./include
)

install(DIRECTORY include DESTINATION .)

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
