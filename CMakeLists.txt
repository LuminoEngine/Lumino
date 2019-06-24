
cmake_minimum_required(VERSION 3.0.0)
project (Lumino)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
enable_testing()

if (ANDROID_ABI)
    set(LN_ANDROID ON)
    message(${ANDROID_SYSROOT})

    if(${ANDROID_ABI} STREQUAL "armeabi-v7a")
        include_directories(${ANDROID_SYSROOT}/usr/include/arm-linux-androideabi)
    elseif(${ANDROID_ABI} STREQUAL "arm64-v8a")
        include_directories(${ANDROID_SYSROOT}/usr/include/aarch64-linux-android)
    elseif(${ANDROID_ABI} STREQUAL "x86")
        include_directories(${ANDROID_SYSROOT}/usr/include/i686-linux-android)
    elseif(${ANDROID_ABI} STREQUAL "x86_64")
        include_directories(${ANDROID_SYSROOT}/usr/include/x86_64-linux-android)
    else()
        include_directories(${ANDROID_SYSROOT}/usr/include/arm-linux-androideabi)
    endif()
endif()


#------------------------------------------------------------------------------
# options

# build unit test projects
option(LN_BUILD_TESTS "build unit test projects." ON)

# build tool projects
option(LN_BUILD_TOOLS "build tool projects." ON)

# build language bindings projects
option(LN_BUILD_BINDINGS "build language bindings." OFF)

# build editor projects
option(LN_BUILD_EDITOR "build editor projects." OFF)

# build only LuminoCore.
option(LN_BUILD_CORE_ONLY "build only LuminoCore." OFF)

# build embedded shader transcompiler.
option(LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER "build tool projects.build embedded shader transcompiler." OFF)

# MSVC Runtime library.
option(LN_MSVC_STATIC_RUNTIME "static runtime (/MT, /MTd) or DLL runtime (/MD, /MDd) or to link either." OFF)

# use SDL on MSVC
option(LN_MSVC_USE_SDL "use SDL on MSVC" OFF)

# Vulkan
option(LN_USE_VULKAN "Vulkan" ON)

#------------------------------------------------------------------------------

message("LN_TARGET_ARCH:${LN_TARGET_ARCH}")

#------------------------------------------------------------------------------

set(CMAKE_DEBUG_POSTFIX "d")

include(src/LuminoBuildCommon.cmake)

set(LUMINO_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}")
set(LUMINO_INCLUDE_DIRECTORIES
    "${LUMINO_ROOT_PATH}/src/LuminoCore/include"
    "${LUMINO_ROOT_PATH}/src/LuminoEngine/include"
    "${LUMINO_ROOT_PATH}/src/LuminoEditor/Core")





if (DEFINED EMSCRIPTEN)
    include_directories("${EMSCRIPTEN_ROOT_PATH}/system/lib/libcxxabi/include")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
    set(LN_EMSCRIPTEN ON)
    add_definitions(-DLN_GRAPHICS_OPENGLES)
    add_definitions(-DLN_GLFW)
    #add_definitions(-DGLFW_INCLUDE_ES2) # glfw3.h 内で include するヘッダを制御する

    set(LN_USE_SDL ON)

elseif (LN_ANDROID)
    add_definitions(-DLN_GRAPHICS_OPENGLES)

elseif (APPLE AND DEFINED PLATFORM) # PLATFORM is ios-cmake variable.
    set(LN_IOS ON)
    set(LN_USE_OPENAL ON)
    add_definitions(-DLN_GRAPHICS_OPENGLES)
    set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "10.0" CACHE string "iOS Deployment Target")

elseif(WIN32 OR APPLE OR UNIX)
    add_definitions(-DLN_GLFW)
    set(GLFW_INSTALL ON CACHE BOOL "" FORCE)
    set(LN_OS_DESKTOP ON)
    set(LN_USE_OPENAL ON)

endif()


if (MSVC AND LN_MSVC_USE_SDL)
    set(LN_USE_SDL ON)
else()
    set(LN_USE_SDL OFF)
endif()

if (MSVC)
    add_definitions("/MP")
endif()

#------------------------------------------------------------------------------
# gtest

set(LN_INCLUDES_gtest
    "${CMAKE_CURRENT_LIST_DIR}/external/gtest/include"
    "${CMAKE_CURRENT_LIST_DIR}/external/gtest"
    "${CMAKE_CURRENT_LIST_DIR}/external/gtest/src")

set(LN_SOURCES_gtest
    "${CMAKE_CURRENT_LIST_DIR}/external/gtest/src/gtest-all.cc")

set (gtest_force_shared_crt True)
add_subdirectory(build/ExternalSource/googletest)

#------------------------------------------------------------------------------

if (LN_BUILD_CORE_ONLY)
    add_subdirectory(src/LuminoCore)
else()
    set(LN_EXTERNAL_FIND_PATH_MODE "build")
    include(external/ImportExternalLibraries.cmake)
    add_subdirectory(src/LuminoCore)
    add_subdirectory(src/LuminoEngine)
    if (LN_BUILD_BINDINGS)
        add_subdirectory(src/LuminoTranscoder)
        #add_subdirectory(tools/lumino-wg)
    endif()
    if (LN_BUILD_EDITOR)
        add_subdirectory(src/LuminoEditor)
        #add_subdirectory(tools/LuminoStudio)
        #add_subdirectory(tools/Editor)
    endif()
    if (LN_BUILD_TOOLS)
        add_subdirectory(tools/LuminoCLI)
    endif()
endif()
