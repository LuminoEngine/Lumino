project(LuminoC)

#-------------------------------------------------------------------------------
# Emscripten - WebAssembly build
#-------------------------------------------------------------------------------
# LuminoBase and LuminoCore are built as static libraries and linked via
# target_link_libraries. WebGPU-distribution is configured in the root
# CMakeLists.txt and provides a unified webgpu target.
if(EMSCRIPTEN)
    set(LN_WEB_SOURCES
        src/LuminoAPI.cpp
    )

    add_executable(${PROJECT_NAME} ${LN_WEB_SOURCES})
    target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".mjs")

    # LuminoCore transitively provides LuminoBase, tl::expected, and emdawnwebgpu_c.
    target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore)

    # LuminoCore private headers (RHI) needed by LuminoAPI.cpp.
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src
    )

    # Emscripten link flags
    set(LN_EM_LINK_FLAGS "")
    string(APPEND LN_EM_LINK_FLAGS " -sWASM=1")
    string(APPEND LN_EM_LINK_FLAGS " -sMODULARIZE=1")
    string(APPEND LN_EM_LINK_FLAGS " -sEXPORT_NAME=\"${PROJECT_NAME}\"")
    string(APPEND LN_EM_LINK_FLAGS " -sEXPORT_ES6=1")
    string(APPEND LN_EM_LINK_FLAGS " -sENVIRONMENT=web")
    string(APPEND LN_EM_LINK_FLAGS " -sNO_FILESYSTEM=1")
    string(APPEND LN_EM_LINK_FLAGS " -sALLOW_MEMORY_GROWTH=1")
    string(APPEND LN_EM_LINK_FLAGS " -sNO_EXIT_RUNTIME=1")
    string(APPEND LN_EM_LINK_FLAGS " -sDISABLE_EXCEPTION_CATCHING=1")

    # EXPORTED_RUNTIME_METHODS は、ライブラリの関数を呼び出すための機能として、
    # ccall / cwrap を使えるようにするかどうか、という設定。
    # それぞれ Module.call(), Module.cwrap() を使えるようにするかというだけだが、EXPORTED_RUNTIME_METHODS 未指定の場合、これらが使えなくなる。
    # (ちなみに、cwrap は ccall を function() でラップするだけ)
    # ccall を使わない場合は Moudle["_myFunction"].apply(...) のようにすることで C 関数を呼び出すことができる。
    # ただし、ccall には string や array の変換ユーティリティがついている。特に理由が無ければ、 ccall または cwrap を利用するのが無難。
    string(APPEND LN_EM_LINK_FLAGS " -sEXPORTED_RUNTIME_METHODS=\"['ccall','cwrap','HEAPU8','HEAPU32','HEAPF32','UTF8ToString']\"")
    string(APPEND LN_EM_LINK_FLAGS " -sEXPORTED_FUNCTIONS=\"['_malloc','_free']\"")
    # ASYNCIFY is required for WebGPU async operations (RequestAdapter,
    # RequestDevice, etc.) to yield to the browser event loop.
    string(APPEND LN_EM_LINK_FLAGS " -sASYNCIFY=1")
    # 最適化レベルは CMAKE_BUILD_TYPE に連動させる。
    # Debug: ブラウザの開発者ツールでソースマップ付きデバッグができるよう最適化を無効化する。
    # Release: まず -O2 で安定を確認する。将来的に -O3 や -flto (Link Time Optimization)
    #          への引き上げを検討する余地がある。
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        string(APPEND LN_EM_LINK_FLAGS " -O0")
        string(APPEND LN_EM_LINK_FLAGS " -g3")
    else()
        string(APPEND LN_EM_LINK_FLAGS " -O2")
    endif()
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${LN_EM_LINK_FLAGS}")

    return()
endif()

#-------------------------------------------------------------------------------
# Desktop build
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Sources
#-------------------------------------------------------------------------------
file(GLOB LN_LOCAL_SOURCES
    src/pch.hpp
    src/*.cpp
    src/*.hpp
    include/${PROJECT_NAME}/*.h
)
source_group("" FILES ${LN_LOCAL_SOURCES})

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_library(
    ${PROJECT_NAME} SHARED
    ${LN_LOCAL_SOURCES}
)
target_compile_definitions(${PROJECT_NAME} PRIVATE LUMINO_BUILD_DLL)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    LuminoBase
    LuminoCore)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)

# WebGPU
if(LN_ENABLE_WEBGPU)
    target_link_libraries(${PROJECT_NAME} PRIVATE webgpu)
    target_copy_webgpu_binaries(${PROJECT_NAME})
endif()

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------
if(LUMINO_BUILD_TESTS)
    add_subdirectory(test)
endif()

#-------------------------------------------------------------------------------
# Examples
#-------------------------------------------------------------------------------
if(LUMINO_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
