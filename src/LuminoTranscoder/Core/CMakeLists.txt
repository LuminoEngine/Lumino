cmake_minimum_required (VERSION 3.0.0)
project(LuminoTranscoder-Core)

set(LWIG_LLVM_PACKAGE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../build/${LN_TARGET_ARCH}/BuildToolsInstall/llvm)
set(CLANG_DIR ${LWIG_LLVM_PACKAGE_DIR}/lib/cmake/clang)
set(LLVM_DIR ${LWIG_LLVM_PACKAGE_DIR}/lib/cmake/llvm)
find_package(CLANG REQUIRED CONFIG)
find_package(LLVM REQUIRED CONFIG)

file(GLOB LOCAL_SOURCES "*.hpp" "*.cpp")
source_group("Sources" FILES ${LOCAL_SOURCES})

file(GLOB LOCAL_SOURCES_Generators "Generators/*.hpp" "Generators/*.cpp")
source_group("Generators" FILES ${LOCAL_SOURCES_Generators})

add_library(${PROJECT_NAME} STATIC ${LOCAL_SOURCES} ${LOCAL_SOURCES_Generators})
target_include_directories(${PROJECT_NAME} PRIVATE . ${LN_ENGINE_INCLUDE_DIR} ${LUMINO_INCLUDE_DIRECTORIES} ${LWIG_LLVM_PACKAGE_DIR}/include)
target_link_libraries(${PROJECT_NAME} LuminoEngine-static)
target_link_libraries(${PROJECT_NAME} clangToolingCore clangAnalysis clangARCMigrate clangAST clangASTMatchers clangBasic clangCodeGen clangDriver clangDynamicASTMatchers clangEdit clangFormat clangFrontend clangFrontendTool clangIndex clangLex clangParse clangRewrite clangRewriteFrontend clangSema clangSerialization clangTooling LLVMSupport LLVMCore LLVMOption LLVMProfileData LLVMBitReader LLVMMC LLVMMCParser LLVMBinaryFormat Version)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Tools")
ln_add_pch(${PROJECT_NAME} "PCH.hpp" "PCH.cpp")

