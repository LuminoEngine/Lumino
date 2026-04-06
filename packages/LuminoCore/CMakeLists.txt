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
file(GLOB LN_LOCAL_SOURCES_Runtime src/Runtime/*.cpp src/Runtime/*.hpp include/${PROJECT_NAME}/Runtime/*.hpp)
file(GLOB LN_LOCAL_SOURCES_Platform src/Platform/*.cpp src/Platform/*.hpp include/${PROJECT_NAME}/Platform/*.hpp)
file(GLOB LN_LOCAL_SOURCES_Graphics
    src/Graphics/*.cpp
    src/Graphics/*.hpp
    include/${PROJECT_NAME}/Graphics/*.hpp
)
file(GLOB LN_LOCAL_SOURCES_Graphics_vulkan
    src/Graphics/rhi/vulkan/*.cpp
    src/Graphics/rhi/vulkan/*.hpp
)
file(GLOB LN_LOCAL_SOURCES_shaders shaders/*.slang)

source_group("" FILES ${LN_LOCAL_SOURCES_core})
source_group("Runtime" FILES ${LN_LOCAL_SOURCES_Runtime})
source_group("Platform" FILES ${LN_LOCAL_SOURCES_Platform})
source_group("Graphics" FILES ${LN_LOCAL_SOURCES_Graphics})
source_group("Graphics/vulkan" FILES ${LN_LOCAL_SOURCES_Graphics_vulkan})
source_group("shaders" FILES ${LN_LOCAL_SOURCES_shaders})

set(LN_LOCAL_SOURCES
    ${LN_LOCAL_SOURCES_core}
    ${LN_LOCAL_SOURCES_Runtime}
    ${LN_LOCAL_SOURCES_Platform}
    ${LN_LOCAL_SOURCES_Graphics}
    ${LN_LOCAL_SOURCES_Graphics_vulkan}
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
