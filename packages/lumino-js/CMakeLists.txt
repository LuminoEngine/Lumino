project(lumino)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------
set(CMAKE_EXECUTABLE_SUFFIX ".js")
set(VERBOSE 1)
#string(APPEND LOCAL_LINK_FLAGS " --pre-js ${CMAKE_CURRENT_LIST_DIR}/renderer_lib/pre.js")
string(APPEND LOCAL_LINK_FLAGS " -s WASM=2")
string(APPEND LOCAL_LINK_FLAGS " -s USE_GLFW=3")
string(APPEND LOCAL_LINK_FLAGS " -s USE_WEBGL2=1")
string(APPEND LOCAL_LINK_FLAGS " -s FULL_ES3=1")
string(APPEND LOCAL_LINK_FLAGS " -s ALLOW_MEMORY_GROWTH=1")

#string(APPEND LOCAL_LINK_FLAGS " --shell-file ${CMAKE_CURRENT_LIST_DIR}/shell_minimal.html")

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

#string(APPEND LOCAL_LINK_FLAGS " --js-library ${CMAKE_CURRENT_LIST_DIR}/renderer_lib/import_funcs.js")

# set(_LINK_FLAGS "--pre-js ${CMAKE_CURRENT_LIST_DIR}/pre.js")
# set(_LINK_FLAGS "${_LINK_FLAGS} -s WASM=2")
#set(_LINK_FLAGS "${_LINK_FLAGS} -s USE_SDL=2")
#set(_LINK_FLAGS "${_LINK_FLAGS} -s USE_GLFW=3")
#set(_LINK_FLAGS "${_LINK_FLAGS} -s USE_WEBGL2=1")
#set(_LINK_FLAGS "${_LINK_FLAGS} -s FULL_ES2=1")
#set(_LINK_FLAGS "${_LINK_FLAGS} -s FULL_ES3=1")
#set(_LINK_FLAGS "${_LINK_FLAGS} -s ASSERTIONS=1")
#set(_LINK_FLAGS "${_LINK_FLAGS} -s USE_PTHREADS=1")
#set(_LINK_FLAGS "${_LINK_FLAGS} -s PTHREAD_POOL_SIZE=8")
#set(_LINK_FLAGS "${_LINK_FLAGS} -s ALLOW_MEMORY_GROWTH=1")
#set(_LINK_FLAGS "${_LINK_FLAGS} -s WASM_MEM_MAX=512MB")
#set(_LINK_FLAGS "${_LINK_FLAGS} -lopenal")
#set(_LINK_FLAGS "${_LINK_FLAGS} --shared-memory")

# for Debug
string(APPEND LOCAL_LINK_FLAGS " --minify 0")
string(APPEND LOCAL_LINK_FLAGS " -g3")
string(APPEND LOCAL_LINK_FLAGS " --emit-symbol-map")

add_executable(${PROJECT_NAME} src/rlumino-js.cpp)
# target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore LuminoPlatform LuminoGraphics)
# set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS ${LOCAL_LINK_FLAGS})

# Build assets
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:${PROJECT_NAME}> ${CMAKE_CURRENT_LIST_DIR}/dist
#     COMMAND ${LN_LUMINO_CLI_TOOL} build-assets -i ${ASSETS_DIR} -o ${CMAKE_CURRENT_LIST_DIR}/dist
# )
