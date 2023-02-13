cmake_minimum_required (VERSION 3.0.0)
project(LuminoTranscoder-Parser)

# set(LN_LLVM_PACKAGE_DIR ${LUMINO_REPO_ROOT_DIR}/build/${LN_TARGET_ARCH}/BuildToolsInstall/llvm)
# set(CLANG_DIR ${LN_LLVM_PACKAGE_DIR}/lib/cmake/clang)
# set(LLVM_DIR ${LN_LLVM_PACKAGE_DIR}/lib/cmake/llvm)
# find_package(CLANG REQUIRED CONFIG)
# find_package(LLVM REQUIRED CONFIG)

find_package(LLVM CONFIG REQUIRED)
find_package(CLANG REQUIRED CONFIG)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(HandleLLVMOptions)
llvm_map_components_to_libnames(llvm_libs Support Core IRReader )

file(GLOB LOCAL_SOURCES "*.hpp" "*.cpp")
source_group("Sources" FILES ${LOCAL_SOURCES})

add_executable(${PROJECT_NAME} ${LOCAL_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE . ${LN_ENGINE_INCLUDE_DIR} ${LUMINO_INCLUDE_DIRECTORIES} ${LN_LLVM_PACKAGE_DIR}/include)
target_compile_definitions(${PROJECT_NAME} PRIVATE ${LLVM_DEFINITIONS})
target_link_libraries(${PROJECT_NAME} LuminoEngine)
target_link_libraries(${PROJECT_NAME} clangToolingCore clangAnalysis clangAST clangASTMatchers clangBasic clangCodeGen clangDriver clangDynamicASTMatchers clangEdit clangFormat clangFrontend clangFrontendTool clangIndex clangLex clangParse clangRewrite clangRewriteFrontend clangSema clangSerialization clangTooling LLVMSupport LLVMCore LLVMOption LLVMProfileData LLVMBitReader LLVMMC LLVMMCParser LLVMBinaryFormat Version)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Tools")
target_precompile_headers(${PROJECT_NAME} PRIVATE "PCH.hpp")

