project(lumino_core)

#-------------------------------------------------------------------------------
# Dependencies
#-------------------------------------------------------------------------------
find_package(Vulkan QUIET)
if(NOT Vulkan_FOUND)
    find_package(vulkan-headers CONFIG REQUIRED)
endif()

#-------------------------------------------------------------------------------
# Sources
#-------------------------------------------------------------------------------
file(GLOB LN_LOCAL_SOURCES_platform
    src/platform/*.cpp
    src/platform/*.hpp
    include/${PROJECT_NAME}/platform/*.hpp
)
source_group("platform" FILES ${LN_LOCAL_SOURCES_platform})

file(GLOB LN_LOCAL_SOURCES_graphics
    src/graphics/*.cpp
    src/graphics/*.hpp
    include/${PROJECT_NAME}/graphics/*.hpp
    src/graphics/rhi/vulkan/vulkan_backend.hpp
    src/graphics/rhi/vulkan/vulkan_backend.cpp
)
source_group("graphics" FILES ${LN_LOCAL_SOURCES_graphics})

set(LN_LOCAL_SOURCES
    ${LN_LOCAL_SOURCES_platform}
    ${LN_LOCAL_SOURCES_graphics}
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
target_link_libraries(${PROJECT_NAME} PUBLIC lumino_base)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)



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