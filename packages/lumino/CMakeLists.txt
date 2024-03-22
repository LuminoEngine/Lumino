project(lumino)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

file(GLOB LN_SOURCES_FFI include/*.h src/*.cpp)

source_group("lumino" FILES ${LN_SOURCES_FFI})

set(LN_SOURCES
    src/pch.hpp
    ${LN_SOURCES_FFI}
)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------
if(LN_EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    string(APPEND LOCAL_LINK_FLAGS " -s WASM=2")
    string(APPEND LOCAL_LINK_FLAGS " -s NO_FILESYSTEM=1")
    string(APPEND LOCAL_LINK_FLAGS " -s NO_EXIT_RUNTIME=1")
    string(APPEND LOCAL_LINK_FLAGS " -s ALLOW_MEMORY_GROWTH=1")

    # MODULARIZE 有効にすると、次のようにモジュール全体がラップされる。
    #   var Module = function(Module) { /* Emscripten の生成コード */ }
    # これにより PIXI.Sprite のように、名前空間を表すことができる。
    # また webpack などと合わせて使うことができ、import を使うことができるようになる。
    # 無効の場合、全ての定義がグローバルスコープに定義される。ライブラリを作る場合は基本的に有効にしておくべき。
    # デフォルトのモジュール名は Module であるが、これは EXPORT_NAME を指定することで変更できる。
    string(APPEND LOCAL_LINK_FLAGS " -s MODULARIZE=1")
    string(APPEND LOCAL_LINK_FLAGS " -s EXPORT_NAME=\"${PROJECT_NAME}\"")

    # nw.js 上では、ENVIRONMENT_IS_WEB と ENVIRONMENT_IS_NODE が同時に true になるが、node が優先的に処理される。
    # その結果、<scripts> によってロードした .js では __dirname など使えない機能にアクセスしようとする。
    # (emscripten 1.30 台の頃は大丈夫だったようだが…)
    # 次のように指定することで、node に関する処理を削ることができる。
    string(APPEND LOCAL_LINK_FLAGS " -s ENVIRONMENT=web,worker")

    # EXPORTED_RUNTIME_METHODS は、ライブラリの関数を呼び出すための機能として、
    # ccall / cwrap を使えるようにするかどうか、という設定。
    # それぞれ Module.call(), Module.cwrap() を使えるようにするかというだけだが、EXPORTED_RUNTIME_METHODS 未指定の場合、これらが使えなくなる。
    # (ちなみに、cwrap は ccall を function() でラップするだけ)
    # ccall を使わない場合は Moudle["_myFunction"].apply(...) のようにすることで C 関数を呼び出すことができる。
    # ただし、ccall には string や array の変換ユーティリティがついている。特に理由が無ければ、 ccall または cwrap を利用するのが無難。
    string(APPEND LOCAL_LINK_FLAGS " -s EXPORTED_RUNTIME_METHODS=\"['ccall','cwrap','GL']\"")

    # for Debug
    string(APPEND LOCAL_LINK_FLAGS " --minify 0")
    string(APPEND LOCAL_LINK_FLAGS " -g3")
    string(APPEND LOCAL_LINK_FLAGS " --emit-symbol-map")
    set(VERBOSE 1)
endif()

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------

add_executable(${PROJECT_NAME} ${LN_SOURCES})
#add_library(${PROJECT_NAME} SHARED ${LN_SOURCES})
#add_library(lumino::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
target_precompile_headers(${PROJECT_NAME} PRIVATE "src/pch.hpp")
target_include_directories(
    ${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(${PROJECT_NAME} PRIVATE include src) # for PCH
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore LuminoRuntime)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS ${LOCAL_LINK_FLAGS})


#-------------------------------------------------------------------------------
# Installation
#-------------------------------------------------------------------------------
if (LUMINO_INSTALL)
    set(LN_TARGETS_NAME "${PROJECT_NAME}Targets")
    set(LN_CONFIG_NAME "${PROJECT_NAME}Config")

    install(
        TARGETS ${PROJECT_NAME}
        EXPORT LuminoExport
    )

    install(DIRECTORY include DESTINATION .)
endif()

#-------------------------------------------------------------------------------
# Sub-projects
#-------------------------------------------------------------------------------

