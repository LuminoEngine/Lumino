
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

# VCPKG_TARGET_ARCHITECTURE への wasm32 指定は標準ではない。
# このようなキーワードを使う場合、 VCPKG_CHAINLOAD_TOOLCHAIN_FILE を使わないと vcpkg がエラーを出力する。
#set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/toolchain-wasm32-emscripten.cmake)
set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE $ENV{EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)

set(COMMON_FLAGS " -pthread")
# string(APPEND CMAKE_CXX_FLAGS "${COMMON_FLAGS}")
# string(APPEND CMAKE_CXX_FLAGS_DEBUG "${COMMON_FLAGS}")
# string(APPEND CMAKE_CXX_FLAGS_RELEASE "${COMMON_FLAGS}")
# string(APPEND CMAKE_C_FLAGS "${COMMON_FLAGS}")
# string(APPEND CMAKE_C_FLAGS_DEBUG "${COMMON_FLAGS}")
# string(APPEND CMAKE_C_FLAGS_RELEASE "${COMMON_FLAGS}")

