cmake_minimum_required (VERSION 3.0.0)
project(lumino-wg)

set(LWIG_LLVM_PACKAGE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../build/BuildTools/LLVMPackage/${LN_BUILD_TYPE})
set(CLANG_DIR ${LWIG_LLVM_PACKAGE_DIR}/lib/cmake/clang)
set(LLVM_DIR ${LWIG_LLVM_PACKAGE_DIR}/lib/cmake/llvm)
find_package(CLANG REQUIRED CONFIG)
find_package(LLVM REQUIRED CONFIG)


set(LN_SOURCES
    PCH.cpp
    PCH.hpp
    Common.hpp
    Generator.cpp
    Generator.hpp
    Main.cpp
    OutputBuffer.cpp
    OutputBuffer.hpp
    Parser.cpp
    Parser.hpp
    SymbolDatabase.cpp
    SymbolDatabase.hpp
	WrapperIF/FlatCCommon.cpp
	WrapperIF/FlatCCommon.hpp
	WrapperIF/WrapperIFClassesGenerator.cpp
	WrapperIF/WrapperIFClassesGenerator.hpp
	WrapperIF/WrapperIFGenerator.cpp
	WrapperIF/WrapperIFGenerator.hpp
	DotNet/CSCommon.cpp
	DotNet/CSCommon.hpp
	DotNet/CSStructsGenerator.cpp
	DotNet/CSStructsGenerator.hpp
)

source_group("Sources" FILES ${LN_SOURCES})

include_directories(
    .
    ${LUMINO_INCLUDE_DIRECTORIES}
	${LWIG_LLVM_PACKAGE_DIR}/include
)

add_executable(${PROJECT_NAME} ${LN_SOURCES})
target_link_libraries(${PROJECT_NAME} LuminoEngine)
target_link_libraries(${PROJECT_NAME} clangToolingCore clangAnalysis clangARCMigrate clangAST clangASTMatchers clangBasic clangCodeGen clangDriver clangDynamicASTMatchers clangEdit clangFormat clangFrontend clangFrontendTool clangIndex clangLex clangParse clangRewrite clangRewriteFrontend clangSema clangSerialization clangTooling LLVMSupport LLVMCore LLVMOption LLVMProfileData LLVMBitReader LLVMMC LLVMMCParser LLVMBinaryFormat Version)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "PCH.hpp" "PCH.cpp")

