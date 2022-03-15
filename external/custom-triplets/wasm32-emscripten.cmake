#
# -pthread オプションを追加してビルドするための Toolchain.
# 
# 実装は <vcpkg>/triplet/community/wasm32-emscripten.cmake をベースにしている。
# ここで VCPKG_TARGET_ARCHITECTURE に wasm32 が指定されているが、
# 標準ではないアーキテクチャを使う場合、 VCPKG_CHAINLOAD_TOOLCHAIN_FILE を使わないと vcpkg がエラーを出力する。
#
# コンパイラに渡すオプションは VCPKG_CXX_FLAGS で指定できるのだが、これは VCPKG_CHAINLOAD_TOOLCHAIN_FILE と併用できない。
# そのため 自作の Toolchain ファイルでオプションを追加し、 Emscripten の Toolchain (Emscripten.cmake) をラップして使う。
#

set(VCPKG_ENV_PASSTHROUGH EMSDK PATH)

if(NOT DEFINED ENV{EMSDK})
   message(FATAL_ERROR "The EMSDK environment variable must be defined")
endif()

if(NOT EXISTS $ENV{EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)
   message(FATAL_ERROR "Emscripten.cmake toolchain file not found")
endif()

set(VCPKG_TARGET_ARCHITECTURE wasm32)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_CMAKE_SYSTEM_NAME Emscripten)
set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/toolchain-wasm32-emscripten.cmake)

