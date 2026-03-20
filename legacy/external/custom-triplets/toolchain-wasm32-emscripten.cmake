#set(VCPKG_ENV_PASSTHROUGH EMSDK PATH)
include($ENV{EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)

set(COMMON_FLAGS " -pthread")
string(APPEND CMAKE_CXX_FLAGS "${COMMON_FLAGS}")
string(APPEND CMAKE_CXX_FLAGS_DEBUG "${COMMON_FLAGS}")
string(APPEND CMAKE_CXX_FLAGS_RELEASE "${COMMON_FLAGS}")
string(APPEND CMAKE_C_FLAGS "${COMMON_FLAGS}")
string(APPEND CMAKE_C_FLAGS_DEBUG "${COMMON_FLAGS}")
string(APPEND CMAKE_C_FLAGS_RELEASE "${COMMON_FLAGS}")

# -pthread を追加すると、cmake の check_type_size が不正な値を返すようになってしまう。(int16_t が 7Byte とかを返す)
# この問題は emscripte の Issue に上がっている。
# https://github.com/emscripten-core/emscripten/issues/15522
# https://github.com/emscripten-core/emscripten/pull/15590
# しかし上記修正は emsdk に組み込まれている emcmake を使った場合のみ有効となるため、
# 他の cmake を使うvcpkg で同様のオプションを使うためには、ここで直接指定する必要がある。
string(APPEND CMAKE_CROSSCOMPILING_EMULATOR ";--experimental-wasm-threads")
