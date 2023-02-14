
cmake_minimum_required(VERSION 3.16.0)
project(Lumino)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
enable_testing()

if(ANDROID_ABI)
    set(LN_ANDROID ON)
endif()

# Determine if fmt is built as a subproject (using add_subdirectory) or if it is the master project.
if(NOT DEFINED LUMINO_MASTER_PROJECT)
    set(LUMINO_MASTER_PROJECT OFF)

    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(LUMINO_MASTER_PROJECT ON)
        message(STATUS "CMake version: ${CMAKE_VERSION}")
    endif()
endif()

# -------------------------------------------------------------------------------
# Options
# -------------------------------------------------------------------------------
option(LN_TARGET_ARCH "arch." "Unknown")

option(LN_BUILD_TESTS "build unit test projects." ON)

option(LUMINO_BUILD_EXAMPLES "build example projects." ON)

# build tool projects
# option(LN_BUILD_TOOLS "build tool projects." ON)

# build language bindings projects
option(LN_BUILD_BINDINGS "build language bindings." OFF)

# build editor projects
option(LN_BUILD_EDITOR "build editor projects." OFF)

# build shared library
option(LN_BUILD_SHARED_LIBRARY "build shared library." OFF)

# build LuminoCore.
option(LUMINO_BUILD_CORE "build LuminoCore." ON)

# build LuminoEngine.
option(LUMINO_BUILD_ENGINE "build LuminoEngine." ON)

# build embedded shader transcompiler.
option(LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER "build tool projects.build embedded shader transcompiler." ON)

# MSVC Runtime library.
option(LN_MSVC_STATIC_RUNTIME "static runtime (/MT, /MTd) or DLL runtime (/MD, /MDd) or to link either." OFF)

# use SDL on MSVC
option(LN_MSVC_USE_SDL "use SDL on MSVC" OFF)

# Vulkan
option(LN_USE_VULKAN "Vulkan" ON)

# OpenGL
option(LN_USE_OPENGL "OpenGL" ON)

# FBX
option(LN_FBX_SDK_PATH "FBX" "")

# Effekseer
option(LN_USE_Effekseer "Effekseer" OFF)

# HSP
option(LN_BUILD_BINDING_HSP "HSP Binding" "")

option(LN_EXTERNAL2 "" OFF)

set(LN_VCPKG_INSTALLED_DIR "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")

option(LUMINO_INSTALL "Generate the install target." ${LUMINO_MASTER_PROJECT})

# -------------------------------------------------------------------------------
# Configuration
# -------------------------------------------------------------------------------
include(GNUInstallDirs)
set(LN_EXPORT_DESTINATION_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/lumino")

set(CMAKE_DEBUG_POSTFIX "d")

# ビルド時にリポジトリ全体に共通設定を伝播させるため、ここで、 include が必要。
include(lumino/LuminoCore/cmake/LuminoConfig-Common.cmake)

if(LN_MSVC_STATIC_RUNTIME)
    ln_use_msvc_static_runtime_library()
endif()

set(LUMINO_REPO_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")
add_definitions(-DLUMINO_REPO_ROOT_DIR="${LUMINO_REPO_ROOT_DIR}")
set(LUMINO_INCLUDE_DIRECTORIES
    "${LUMINO_REPO_ROOT_DIR}/src/Core/include"
    "${LUMINO_REPO_ROOT_DIR}/src/Engine/include"
    "${LUMINO_REPO_ROOT_DIR}/src/Editor/Core")

if(LN_FBX_SDK_PATH)
    message("LN_FBX_SDK_PATH: ${LN_FBX_SDK_PATH}")
    set(LN_USE_FBX_IMPORTER ON)
endif()

if(DEFINED EMSCRIPTEN)
    include_directories("${EMSCRIPTEN_ROOT_PATH}/system/lib/libcxxabi/include")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
    set(LN_EMSCRIPTEN ON)
    add_definitions(-DLN_GRAPHICS_OPENGLES)
    add_definitions(-DLN_GLFW)

    # add_definitions(-DGLFW_INCLUDE_ES2) # glfw3.h 内で include するヘッダを制御する
    set(LN_USE_SDL ON)
    set(LN_USE_VULKAN OFF)
    set(LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER OFF)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

    # set_target_properties(LuminoEngine PROPERTIES COMPILE_FLAGS "-pthread")
    if(LUMINO_BUILD_EXAMPLES)
        set(LN_LUMINO_CLI_TOOL "${CMAKE_CURRENT_LIST_DIR}/build/buildtrees/x64-windows-static/lumino/src/Editor/CLI/Debug/lumino.exe")

        if(NOT EXISTS ${LN_LUMINO_CLI_TOOL})
            message(FATAL_ERROR "Not found: ${LN_LUMINO_CLI_TOOL}")
        endif()
    endif()

elseif(LN_ANDROID)
    add_definitions(-DLN_GRAPHICS_OPENGLES)

elseif(APPLE AND DEFINED PLATFORM) # PLATFORM is ios-cmake variable.
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

if(MSVC)
    if(LN_MSVC_USE_SDL)
        set(LN_USE_SDL ON)
    else()
        set(LN_USE_SDL OFF)
    endif()

    add_definitions("/MP")

    # TODO: lib 分割したい
    add_definitions("/bigobj")
endif()

if(LUMINO_BUILD_ENGINE)
    set(LUMINO_BUILD_CORE ON)
endif()

# -------------------------------------------------------------------------------
# Externals
# -------------------------------------------------------------------------------
if(LN_OS_DESKTOP)
    find_package(GTest CONFIG REQUIRED)
endif()

# ------------------------------------------------------------------------------
# gtest
set(LN_INCLUDES_gtest
    "${CMAKE_CURRENT_LIST_DIR}/build/ExternalSource/googletest/googletest/include"
    "${CMAKE_CURRENT_LIST_DIR}/build/ExternalSource/googletest/googletest"
    "${CMAKE_CURRENT_LIST_DIR}/build/ExternalSource/googletest/googletest/src")

set(LN_SOURCES_gtest
    "${CMAKE_CURRENT_LIST_DIR}/build/ExternalSource/googletest/googletest/src/gtest-all.cc")

set(LN_GLTF_SAMPLE_MODELS_DIR "${CMAKE_CURRENT_LIST_DIR}/build/ExternalSource/glTF-Sample-Models/")

# ------------------------------------------------------------------------------
if(LUMINO_BUILD_CORE)
    add_subdirectory(lumino/LuminoCore)
endif()

if(LUMINO_BUILD_ENGINE)
    if(LN_EMSCRIPTEN)
        add_subdirectory(lumino/Runtime)
        add_subdirectory(lumino/Platform)
        add_subdirectory(lumino/Graphics)
        add_subdirectory(lumino/LuminoEngine)
        add_subdirectory(examples/Example-WebLibrary)
    else()
        set(LN_EXTERNAL_FIND_PATH_MODE "build")
        include(external/ImportExternalLibraries.cmake)
        add_subdirectory(lumino/Runtime)
        add_subdirectory(lumino/Platform)
        add_subdirectory(lumino/Graphics)
        add_subdirectory(lumino/LuminoEngine)
        add_subdirectory(lumino/FFI)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT LuminoEngine-sandbox)

        if(LUMINO_BUILD_EXAMPLES)
            # add_subdirectory(examples)
        endif()

        if(LN_BUILD_BINDINGS)
            add_subdirectory(src/Transcoder)
        endif()

        if(LN_BUILD_EDITOR)
            add_subdirectory(src/Editor)
        endif()

        if(${LN_TARGET_ARCH} STREQUAL "MSVC2019-x86-MT")
            add_subdirectory(tools/Bindings/HSP3)
        endif()
    endif()
endif()

# -------------------------------------------------------------------------------
# Installation
# -------------------------------------------------------------------------------
# ちょっと複雑になってるので注意。
# このようにした動機は、
# - find_package(Lumino CONFIG REQUIRED) したときに、fmt など外部ライブラリもまとめて find_package したいためである。
# find_package(Lumino CONFIG REQUIRED) では、まず LuminoConfig.cmake を読みに行こうとする。
# 当初はこのファイルは install(EXPORT) コマンドで cmake に自動生成させていたのだが、
# この場合、生成コードをカスタマイズすることができない。
# そのため install(EXPORT) では LuminoTargets.cmake(及び、LuminoTargets-debug.cmake など) を生成させて、
# 自分で作った LuminoConfig.cmake からそれらを include することにしている。
if(LUMINO_INSTALL)
    # configure_package_config_file によって Configure 実行時に、
    # LuminoConfig.cmake を ${CMAKE_CURRENT_BINARY_DIR} へ生成する。
    include(CMakePackageConfigHelpers)
    configure_package_config_file(
        "cmake/LuminoConfig.cmake.in"
        "LuminoConfig.cmake"
        INSTALL_DESTINATION ${LN_EXPORT_DESTINATION_DIR}
    )

    install(
        EXPORT LuminoExport
        NAMESPACE lumino::
        DESTINATION ${LN_EXPORT_DESTINATION_DIR}
        FILE LuminoTargets.cmake
        EXPORT_LINK_INTERFACE_LIBRARIES
    )

    install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/LuminoConfig.cmake"
        DESTINATION ${LN_EXPORT_DESTINATION_DIR}
    )
endif()
