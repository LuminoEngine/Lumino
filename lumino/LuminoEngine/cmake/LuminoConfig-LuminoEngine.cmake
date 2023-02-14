

# find_package(yaml-cpp CONFIG REQUIRED)
find_package(toml11 CONFIG REQUIRED)
find_package(Vorbis CONFIG REQUIRED)
find_package(Bullet CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)
find_package(nanovg CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb_image.h")
find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")
find_package(imgui CONFIG REQUIRED)

if (LN_OS_DESKTOP)
    find_package(GTest CONFIG REQUIRED)
    find_package(OpenAL CONFIG REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(glad CONFIG REQUIRED)
    find_path(VULKAN_HEADERS_INCLUDE_DIRS "vulkan/vk_icd.h")
endif()

#-------------------------------------------------------------------------------
# pcre2
#-------------------------------------------------------------------------------
if (NOT LN_VCPKG_INSTALLED_DIR)
    # 外部の vcpkg と一緒に使うときに LN_VCPKG_INSTALLED_DIR を定義しなければならないのは忘れやすい。
    # _VCPKG_INSTALLED_DIR は非公開変数ではあるようだが、(https://github.com/Microsoft/vcpkg/issues/3915)
    # zlib\vcpkg-cmake-wrapper.cmake で使っているので真似してみる。
    set(pcre2_ROOT "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")
else()
    set(pcre2_ROOT ${LN_VCPKG_INSTALLED_DIR})
endif()

# Emscripten での find_package(Lumino) 時は、 SYSTEM_PATH とみなされている vcpkg からも探したい。
# というよりそもそも Emscripten 環境下では、 Emscripten のツールチェインファイルにより、
# SYSTEM_PATH 以外のライブラリ検索が禁止されている。
find_library(pcre2_32_LIBRARY_RELEASE NAMES libpcre2-32 pcre2-32 PATHS ${pcre_ROOT} PATH_SUFFIXES "lib")
find_library(pcre2_32_LIBRARY_DEBUG NAMES libpcre2-32d libpcre2-32 pcre2-32d pcre2-32 PATHS ${pcre_ROOT} PATH_SUFFIXES "debug/lib")

# 見つからなければ、今度は -static を付けて検索してみる。
# これは、古い vcpkg を使っていたときに必要だった。
if (NOT pcre2_32_LIBRARY_RELEASE)
    find_library(pcre2_32_LIBRARY_RELEASE NAMES pcre2-32-static PATHS ${pcre_ROOT} PATH_SUFFIXES "lib" NO_CMAKE_SYSTEM_PATH)
    find_library(pcre2_32_LIBRARY_DEBUG NAMES pcre2-32-staticd pcre2-32-static PATHS ${pcre_ROOT} PATH_SUFFIXES "debug/lib" NO_CMAKE_SYSTEM_PATH)
endif()

add_library(pcre2_32 STATIC IMPORTED)
set_target_properties(pcre2_32 PROPERTIES IMPORTED_LOCATION ${pcre2_32_LIBRARY_RELEASE})
set_target_properties(pcre2_32 PROPERTIES IMPORTED_LOCATION_RELEASE ${pcre2_32_LIBRARY_RELEASE})
set_target_properties(pcre2_32 PROPERTIES IMPORTED_LOCATION_DEBUG ${pcre2_32_LIBRARY_DEBUG})
set_target_properties(pcre2_32 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${pcre2_ROOT}/include)

