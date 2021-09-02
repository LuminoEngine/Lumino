
cmake_minimum_required(VERSION 3.0.0)
project (Lumino)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
enable_testing()

if (ANDROID_ABI)
    set(LN_ANDROID ON)
endif()

#------------------------------------------------------------------------------
# options

option(LN_TARGET_ARCH "arch." "Unknown")

# build unit test projects
option(LN_BUILD_TESTS "build unit test projects." ON)

# build tool projects
#option(LN_BUILD_TOOLS "build tool projects." ON)

# build language bindings projects
option(LN_BUILD_BINDINGS "build language bindings." OFF)

# build editor projects
option(LN_BUILD_EDITOR "build editor projects." OFF)

# build shared library
option(LN_BUILD_SHARED_LIBRARY "build shared library." OFF)

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

# FBX
option(LN_FBX_SDK_PATH "FBX" "")

# Effekseer
option(LN_USE_Effekseer "Effekseer" OFF)

# HSP
option(LN_BUILD_BINDING_HSP "HSP Binding" "")

#------------------------------------------------------------------------------

message("LN_TARGET_ARCH:${LN_TARGET_ARCH}")

#------------------------------------------------------------------------------

set(CMAKE_DEBUG_POSTFIX "d")

include(src/LuminoBuildCommon.cmake)

set(LUMINO_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}")
set(LUMINO_INCLUDE_DIRECTORIES
    "${LUMINO_ROOT_PATH}/src/Core/include"
    "${LUMINO_ROOT_PATH}/src/Engine/include"
    "${LUMINO_ROOT_PATH}/src/Editor/Core")

if (LN_FBX_SDK_PATH)
    message("LN_FBX_SDK_PATH: ${LN_FBX_SDK_PATH}")
    set(LN_USE_FBX_IMPORTER ON)
endif()


if (DEFINED EMSCRIPTEN)
    include_directories("${EMSCRIPTEN_ROOT_PATH}/system/lib/libcxxabi/include")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
    set(LN_EMSCRIPTEN ON)
    add_definitions(-DLN_GRAPHICS_OPENGLES)
    add_definitions(-DLN_GLFW)
    #add_definitions(-DGLFW_INCLUDE_ES2) # glfw3.h 内で include するヘッダを制御する

    set(LN_USE_SDL ON)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    #set_target_properties(LuminoEngine-static PROPERTIES COMPILE_FLAGS "-pthread")

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


if (MSVC)
    if (LN_MSVC_USE_SDL)
        set(LN_USE_SDL ON)
    else()
        set(LN_USE_SDL OFF)
    endif()
    add_definitions("/MP")

    set(LN_COMMON_META_FILES "${CMAKE_CURRENT_LIST_DIR}/tools/VisualStudio/Lumino.natvis")
endif()

#------------------------------------------------------------------------------
# gtest

set(LN_INCLUDES_gtest
    "${CMAKE_CURRENT_LIST_DIR}/build/ExternalSource/googletest/googletest/include"
    "${CMAKE_CURRENT_LIST_DIR}/build/ExternalSource/googletest/googletest"
    "${CMAKE_CURRENT_LIST_DIR}/build/ExternalSource/googletest/googletest/src")

set(LN_SOURCES_gtest
    "${CMAKE_CURRENT_LIST_DIR}/build/ExternalSource/googletest/googletest/src/gtest-all.cc")

set(LN_GLTF_SAMPLE_MODELS_DIR "${CMAKE_CURRENT_LIST_DIR}/build/ExternalSource/glTF-Sample-Models/")

#------------------------------------------------------------------------------

if (LN_BUILD_CORE_ONLY)
    add_subdirectory(src/Core)
else()
    set(LN_EXTERNAL_FIND_PATH_MODE "build")
    include(external/ImportExternalLibraries.cmake)
    add_subdirectory(src/Core)
    add_subdirectory(src/Engine)
    if (LN_BUILD_TESTS)
        add_subdirectory(examples)
    endif()
    if (LN_BUILD_BINDINGS)
        add_subdirectory(src/Transcoder)
    endif()
    if (LN_BUILD_EDITOR)
        add_subdirectory(src/Editor)
    endif()
    if (${LN_TARGET_ARCH} STREQUAL "MSVC2019-x86-MT")
        add_subdirectory(tools/Bindings/HSP3)
    endif()
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT LuminoEngine-sandbox)
