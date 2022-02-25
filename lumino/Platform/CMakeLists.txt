
cmake_minimum_required(VERSION 3.0)
project(LuminoPlatform)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

file(GLOB LN_SOURCES_Platform include/LuminoPlatform/*.hpp src/Platform/*.cpp src/Platform/*.hpp)

if (LN_EMSCRIPTEN)
elseif (WIN32)
    set(LN_SOURCES_Platform
        ${LN_SOURCES_Platform}
        include/LuminoPlatform/Win32PlatformInterface.hpp
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

# Grouping.
source_group("Platform" FILES ${LN_SOURCES_Platform})

set(LN_SOURCES
    src/pch.cpp
    src/pch.hpp
    src/Internal.hpp
    ${LN_SOURCES_Platform}
)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------


#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
#target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)
#target_include_directories(${PROJECT_NAME} PUBLIC ${PUBLIC_HDR_DIR})
#target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
#target_include_directories(${PROJECT_NAME} PUBLIC include)
#set_target_properties(${PROJECT_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(
    ${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(${PROJECT_NAME} PRIVATE include src) # for PCH
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore)
target_link_libraries(${PROJECT_NAME} PUBLIC LuminoRuntimeCore)

if (LN_OS_DESKTOP)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad::glad)
endif()
if (LN_EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-pthread")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/pch.hpp" "src/pch.cpp")

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

if (LN_BUILD_TESTS)
    add_subdirectory(examples/Windows)
endif()

if (LN_EMSCRIPTEN)
    add_subdirectory(examples/Web)
endif()
