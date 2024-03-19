
cmake_minimum_required(VERSION 3.0)
project(LuminoRuntime)
include(cmake/LuminoConfig-LuminoRuntime.cmake)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------
set(LN_SOURCES_Common
    include/LuminoEngine/RuntimeModule.hpp
    src/pch.hpp
    src/Internal.hpp
    src/RegisterTypes.cpp
    src/RuntimeModule.cpp
)
file(GLOB LN_SOURCES_Runtime include/LuminoEngine/Runtime/*.hpp include/LuminoEngine/Runtime/detail/*.hpp src/Runtime/*.cpp src/Runtime/*.hpp)
file(GLOB LN_SOURCES_Base include/LuminoEngine/Base/*.hpp src/Base/*.cpp src/Base/*.hpp)
file(GLOB LN_SOURCES_Reflection include/LuminoEngine/Reflection/*.hpp src/Reflection/*.cpp src/Reflection/*.hpp)
file(GLOB LN_SOURCES_Engine include/LuminoEngine/Engine/*.hpp src/Engine/*.cpp src/Engine/*.hpp)
file(GLOB LN_SOURCES_Asset include/LuminoEngine/Asset/*.hpp src/Asset/*.cpp src/Asset/*.hpp)
file(GLOB LN_SOURCES_Graphics include/LuminoEngine/Graphics/*.hpp src/Graphics/*.cpp src/Graphics/*.hpp)
file(GLOB LN_SOURCES_Platform include/LuminoEngine/*.hpp include/LuminoEngine/detail/*.hpp src/Platform/*.cpp src/Platform/*.hpp)
if (LN_EMSCRIPTEN)
elseif (WIN32)
    set(LN_SOURCES_Platform
        ${LN_SOURCES_Platform}
        include/LuminoEngine/Platform/Win32PlatformInterface.hpp
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
source_group("" FILES ${LN_SOURCES_Common})
source_group("Runtime" FILES ${LN_SOURCES_Runtime})
source_group("Base" FILES ${LN_SOURCES_Base})
source_group("Reflection" FILES ${LN_SOURCES_Reflection})
source_group("Engine" FILES ${LN_SOURCES_Engine})
source_group("Asset" FILES ${LN_SOURCES_Asset})
source_group("Graphics" FILES ${LN_SOURCES_Graphics})
source_group("Platform" FILES ${LN_SOURCES_Platform})

set(LN_SOURCES
    ${LN_SOURCES_Common}
    ${LN_SOURCES_Runtime}
    ${LN_SOURCES_Base}
    ${LN_SOURCES_Reflection}
    ${LN_SOURCES_Engine}
    ${LN_SOURCES_Asset}
    ${LN_SOURCES_Graphics}
    ${LN_SOURCES_Platform}
)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------
set(LINK_LIBS
    LuminoCore
    yaml-cpp
)

#if (NOT DEFINED EMSCRIPTEN)
#    list(APPEND LINK_LIBS CURL::libcurl)
#endif()

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
add_library(lumino::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(
    ${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(${PROJECT_NAME} PRIVATE include src)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LINK_LIBS})

if (LN_OS_DESKTOP)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad::glad)
endif()
if (LN_EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-pthread")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
target_precompile_headers(${PROJECT_NAME} PRIVATE "src/pch.hpp")

#-------------------------------------------------------------------------------
# Installation
#-------------------------------------------------------------------------------
if (LUMINO_INSTALL)
    install(
        TARGETS     ${PROJECT_NAME}
        EXPORT      LuminoExport
    )

    install(DIRECTORY include DESTINATION .)

    # install(EXPORT) で出力される LuminoCoreConfig.cmake は、同一フォルダの
    # LuminoCoreConfig-*.cmake を include しようとするため、それにマッチするファイル名にする。
    install(FILES cmake/LuminoConfig-${PROJECT_NAME}.cmake DESTINATION ${LN_EXPORT_DESTINATION_DIR})
endif()

#-------------------------------------------------------------------------------
# Sub-projects
#-------------------------------------------------------------------------------
if (LN_BUILD_TESTS)
    add_subdirectory(test)
endif()

