

find_package(yaml-cpp CONFIG REQUIRED)
find_package(toml11 CONFIG REQUIRED)
find_package(Vorbis CONFIG REQUIRED)
find_package(Bullet CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)
find_package(nanovg CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb_image.h")
find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")

if (LN_OS_DESKTOP)
    find_package(glslang CONFIG REQUIRED)
    #find_package(spirv_cross_c CONFIG REQUIRED)
    find_package(spirv_cross_core CONFIG REQUIRED)
    #find_package(spirv_cross_cpp CONFIG REQUIRED)
    find_package(spirv_cross_glsl CONFIG REQUIRED)
    find_package(spirv_cross_hlsl CONFIG REQUIRED)
    find_package(spirv_cross_msl CONFIG REQUIRED)
    find_package(spirv_cross_reflect CONFIG REQUIRED)
    find_package(spirv_cross_util CONFIG REQUIRED)

    find_package(GTest CONFIG REQUIRED)
    find_package(OpenAL CONFIG REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(glad CONFIG REQUIRED)
    find_path(VULKAN_HEADERS_INCLUDE_DIRS "vulkan/vk_icd.h")
    find_package(imgui CONFIG REQUIRED)
endif()

# pcre2
set(pcre2_ROOT ${LN_VCPKG_INSTALLED_DIR})
find_library(pcre2_32_LIBRARY_RELEASE NAMES pcre2-32 PATHS ${pcre_ROOT} PATH_SUFFIXES "lib" NO_CMAKE_SYSTEM_PATH)
find_library(pcre2_32_LIBRARY_DEBUG NAMES pcre2-32d pcre2-32 PATHS ${pcre_ROOT} PATH_SUFFIXES "debug/lib" NO_CMAKE_SYSTEM_PATH)
add_library(pcre2_32 STATIC IMPORTED)
set_target_properties(pcre2_32 PROPERTIES IMPORTED_LOCATION_RELEASE ${pcre2_32_LIBRARY_RELEASE})
set_target_properties(pcre2_32 PROPERTIES IMPORTED_LOCATION_DEBUG ${pcre2_32_LIBRARY_DEBUG})
set_target_properties(pcre2_32 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${pcre2_ROOT}/include)

