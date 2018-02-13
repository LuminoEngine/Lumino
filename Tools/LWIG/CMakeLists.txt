
#set(LWIG_LLVM_PACKAGE_DIR D:/Proj/Tech/llvm/1_Package_release_60/Debug)
set(LWIG_LLVM_PACKAGE_DIR C:/Proj/llvm/build_debug/package)
set(CLANG_DIR ${LWIG_LLVM_PACKAGE_DIR}/lib/cmake/clang)
set(LLVM_DIR ${LWIG_LLVM_PACKAGE_DIR}/lib/cmake/llvm)
find_package(CLANG REQUIRED CONFIG)
find_package(LLVM REQUIRED CONFIG)

set(LN_SOURCES
	LWIG.PCH.cpp
	LWIG.PCH.h
	Main.cpp
	Common.h
	Generator.h
	OutputBuffer.cpp
	OutputBuffer.h
	Parser.cpp
	Parser.h
	SymbolDatabase.cpp
	SymbolDatabase.h
	WrapperIF/WrapperIFClassesGenerator.cpp
	WrapperIF/WrapperIFClassesGenerator.h
	WrapperIF/WrapperIFGenerator.cpp
	WrapperIF/WrapperIFGenerator.h
	DotNet/CSCommon.cpp
	DotNet/CSCommon.h
	DotNet/CSStructsGenerator.cpp
	DotNet/CSStructsGenerator.h
)

source_group("Source"	FILES ${LN_SOURCES})

include_directories(
	"${LWIG_LLVM_PACKAGE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/Source/LuminoCore/Include"
	"${PROJECT_SOURCE_DIR}/Source/Fluorite/Include"
)

add_executable(LWIG ${LN_SOURCES})
target_link_libraries(LWIG LuminoEngine)
target_link_libraries(LWIG Fluorite)
target_link_libraries(LWIG clangToolingCore clangAnalysis clangARCMigrate clangAST clangASTMatchers clangBasic clangCodeGen clangDriver clangDynamicASTMatchers clangEdit clangFormat clangFrontend clangFrontendTool clangIndex clangLex clangParse clangRewrite clangRewriteFrontend clangSema clangSerialization clangTooling LLVMSupport LLVMCore LLVMOption LLVMProfileData LLVMBitReader LLVMMC LLVMMCParser LLVMBinaryFormat Version)
ln_add_pch(LWIG "LWIG.PCH.h" "LWIG.PCH.cpp")
