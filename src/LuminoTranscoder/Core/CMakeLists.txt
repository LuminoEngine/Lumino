cmake_minimum_required (VERSION 3.0.0)
project(LuminoTranscoder-Core)

set(LWIG_LLVM_PACKAGE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../build/BuildTools/LLVMPackage/${LN_BUILD_TYPE})
set(CLANG_DIR ${LWIG_LLVM_PACKAGE_DIR}/lib/cmake/clang)
set(LLVM_DIR ${LWIG_LLVM_PACKAGE_DIR}/lib/cmake/llvm)
find_package(CLANG REQUIRED CONFIG)
find_package(LLVM REQUIRED CONFIG)

file(GLOB_RECURSE LOCAL_SOURCES "*.hpp" "*.cpp")
source_group("Sources" FILES ${LOCAL_SOURCES})

add_library(${PROJECT_NAME} STATIC ${LOCAL_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE . ${LN_ENGINE_INCLUDE_DIR} ${LUMINO_INCLUDE_DIRECTORIES} ${LWIG_LLVM_PACKAGE_DIR}/include)
target_link_libraries(${PROJECT_NAME} LuminoEngine)
target_link_libraries(${PROJECT_NAME} clangToolingCore clangAnalysis clangARCMigrate clangAST clangASTMatchers clangBasic clangCodeGen clangDriver clangDynamicASTMatchers clangEdit clangFormat clangFrontend clangFrontendTool clangIndex clangLex clangParse clangRewrite clangRewriteFrontend clangSema clangSerialization clangTooling LLVMSupport LLVMCore LLVMOption LLVMProfileData LLVMBitReader LLVMMC LLVMMCParser LLVMBinaryFormat Version)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Tools")
ln_add_pch(${PROJECT_NAME} "PCH.hpp" "PCH.cpp")

