project(Experimental_Graphics_ExternalOpenGLContext)


#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------
set(LOCAL_LINK_LIBRARIES LuminoCore LuminoRuntime LuminoGraphics)

if (LN_EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    string(APPEND LOCAL_LINK_FLAGS " --pre-js ${CMAKE_CURRENT_LIST_DIR}/pre.js")
    string(APPEND LOCAL_LINK_FLAGS " -s WASM=2")
    string(APPEND LOCAL_LINK_FLAGS " -s USE_GLFW=3")
    string(APPEND LOCAL_LINK_FLAGS " -s USE_WEBGL2=1")
    string(APPEND LOCAL_LINK_FLAGS " -s FULL_ES3=1")

    # 今のところ、Lumino のランタイムは消費メモリがかなり大きい。普通に実行すると次のようなエラーが発生する。
    # Uncaught RuntimeError: abort(Cannot enlarge memory arrays to size 16781312 bytes (OOM). Either
    #   (1) compile with  -s INITIAL_MEMORY=X  with X higher than the current value 16777216,
    #   (2) compile with  -s ALLOW_MEMORY_GROWTH=1  which allows increasing the size at runtime, or
    #   (3) if you want malloc to return NULL (0) instead of this abort, compile with  -s ABORTING_MALLOC=0 ) at Error
    string(APPEND LOCAL_LINK_FLAGS " -s ALLOW_MEMORY_GROWTH=1")

    # for Debug
    string(APPEND LOCAL_LINK_FLAGS " --minify 0")
    #string(APPEND LOCAL_LINK_FLAGS " -gsource-map")
    string(APPEND LOCAL_LINK_FLAGS " -g3")
    string(APPEND LOCAL_LINK_FLAGS " --emit-symbol-map")
else()
    find_package(glad CONFIG REQUIRED)
    list(APPEND LOCAL_LINK_LIBRARIES glad::glad)
endif()

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} Main.cpp)
target_compile_definitions(${PROJECT_NAME} PRIVATE ASSETS_DIR=U"${ASSETS_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE ${LOCAL_LINK_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Examples")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "index")

if (LN_EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS ${LOCAL_LINK_FLAGS})

    # Build assets and copy test server.
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${LN_LUMINO_CLI_TOOL} build-assets -i ${ASSETS_DIR} -o $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/serve.py $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()
