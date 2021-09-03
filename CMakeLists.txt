
cmake_minimum_required(VERSION 3.0.0)
project (Lumino)


#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------

# build unit test projects
option(LN_BUILD_TESTS "build unit test projects." ON)

# build embedded shader transcompiler.
option(LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER "build tool projects.build embedded shader transcompiler." OFF)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_CXX_EXTENSIONS OFF)
set(LN_VCPKG_INCLUDE_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")

#set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/_build/vcpkg/installed/x64-windows")
find_package(GTest CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

find_package(yaml-cpp CONFIG REQUIRED)
find_package(toml11 CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(Vorbis CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(libpng CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(nanovg CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)
find_package(Bullet CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb_image.h")
find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")
find_path(VULKAN_HEADERS_INCLUDE_DIRS "vk_video/vulkan_video_codec_h264std.h")


find_package(glslang CONFIG REQUIRED)
find_package(spirv_cross_c CONFIG REQUIRED)
find_package(spirv_cross_core CONFIG REQUIRED)
find_package(spirv_cross_cpp CONFIG REQUIRED)
find_package(spirv_cross_glsl CONFIG REQUIRED)
find_package(spirv_cross_hlsl CONFIG REQUIRED)
find_package(spirv_cross_msl CONFIG REQUIRED)
find_package(spirv_cross_reflect CONFIG REQUIRED)
find_package(spirv_cross_util CONFIG REQUIRED)




#--------------------------------------
# pcre2
set(pcre2_ROOT ${LN_VCPKG_INCLUDE_DIR})

find_library(pcre2_16_LIBRARY_RELEASE NAMES pcre2-16 PATHS ${pcre_ROOT} PATH_SUFFIXES "lib" NO_CMAKE_SYSTEM_PATH)
find_library(pcre2_16_LIBRARY_DEBUG NAMES pcre2-16d pcre2-16 PATHS ${pcre_ROOT} PATH_SUFFIXES "debug/lib" NO_CMAKE_SYSTEM_PATH)

add_library(pcre2_16 STATIC IMPORTED)
set_target_properties(pcre2_16 PROPERTIES IMPORTED_LOCATION_RELEASE ${pcre2_16_LIBRARY_RELEASE})
set_target_properties(pcre2_16 PROPERTIES IMPORTED_LOCATION_DEBUG ${pcre2_16_LIBRARY_DEBUG})
set_target_properties(pcre2_16 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${pcre2_ROOT}/include)
#list(APPEND LN_EXTERNAL_LIBS pcre2_16)




set_property(GLOBAL PROPERTY USE_FOLDERS ON)
enable_testing()

add_subdirectory(lumino/LuminoCore)
add_subdirectory(lumino/LuminoEngine)

