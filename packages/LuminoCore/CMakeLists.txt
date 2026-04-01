project(LuminoCore)

#-------------------------------------------------------------------------------
# Dependencies
#-------------------------------------------------------------------------------
find_package(Vulkan QUIET)
if(NOT Vulkan_FOUND)
    find_package(vulkan-headers CONFIG REQUIRED)
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
file(GLOB LN_LOCAL_SOURCES_runtime src/runtime/*.cpp src/runtime/*.hpp include/${PROJECT_NAME}/runtime/*.hpp)
file(GLOB LN_LOCAL_SOURCES_platform src/platform/*.cpp src/platform/*.hpp include/${PROJECT_NAME}/platform/*.hpp)
file(GLOB LN_LOCAL_SOURCES_graphics
    src/graphics/*.cpp
    src/graphics/*.hpp
    include/${PROJECT_NAME}/graphics/*.hpp
)
file(GLOB LN_LOCAL_SOURCES_graphics_vulkan
    src/graphics/rhi/vulkan/*.cpp
    src/graphics/rhi/vulkan/*.hpp
)
file(GLOB LN_LOCAL_SOURCES_shaders shaders/*.slang)

source_group("" FILES ${LN_LOCAL_SOURCES_core})
source_group("runtime" FILES ${LN_LOCAL_SOURCES_runtime})
source_group("platform" FILES ${LN_LOCAL_SOURCES_platform})
source_group("graphics" FILES ${LN_LOCAL_SOURCES_graphics})
source_group("graphics/vulkan" FILES ${LN_LOCAL_SOURCES_graphics_vulkan})
source_group("shaders" FILES ${LN_LOCAL_SOURCES_shaders})

set(LN_LOCAL_SOURCES
    ${LN_LOCAL_SOURCES_core}
    ${LN_LOCAL_SOURCES_runtime}
    ${LN_LOCAL_SOURCES_platform}
    ${LN_LOCAL_SOURCES_graphics}
    ${LN_LOCAL_SOURCES_graphics_vulkan}
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
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoShader glfw)
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})



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

if(LUMINO_PLATFORM_DESKTOP)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
endif()

#-------------------------------------------------------------------------------
# Shader compilation (slang → lcsh → lcsh.inl)
#-------------------------------------------------------------------------------
set(LN_SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)

set(LN_BUILTIN_SHADERS
    Unlit
    BasicLit
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
