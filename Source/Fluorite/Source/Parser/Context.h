
#pragma once
#include <unordered_map>
#include "Common.h"
#include "Frontend\Cpp\Preprocessor.h"

LN_NAMESPACE_BEGIN
namespace parser
{

class CompileOptions
{
public:


	Array<TokenPathName>	m_additionalIncludePaths;	// 追加のインクルードディレクトリ
	RefPtr<MacroMap>		m_preprocessorDefinitions;	// 定義済みマクロ (解析開始時の MacroMap)
	

	//CompileOptions*	m_parent;
};

//struct CacheFileKey
//{
//	uint64_t	includePathsKey = 0;
//	uint64_t	definedMacrosKey = 0;
//};

class Context
	: public RefObject
{
public:
	void Clear();

	// TODO: コンパイラオプションのインクルードファイルの検索パスの並びも一致している必要がある。
	/*
		何か解析済みのファイルがほしいときはこの関数を使う。
		.c などのコンパイル単位となるファイルは、コンパイルオプションでキーを指定。
		.h などのインクルードファイルは#include時点のマクロでキーを指定。
	*/
//	UnitFile* LookupCachedUnitFile(const TokenPathName& fileAbsPath, const CacheFileKey& key);

	ResultState LookupPreprocessedIncludeFile(
		CompileUnitFile* rootCompileUnitFile,
		const TokenPathName& basePath,
		const TokenPathName& filePath,
		const Array<TokenPathName>* additionalIncludePaths,
		const MacroMapContainer& parentMacroMap,
		ReferenceTracker* referenceTracker,
		DiagnosticsItemSet* parentDiag,
		IncludeFile** outFile);

	static uint64_t MakeCacheFileKey(const PathName& includeFilePath, const Array<TokenPathName>* additionalIncludePaths, const MacroMap* macroMap);

	// FontendContext/AnalayzerContext 分けたほうがいい？

private:
	std::unordered_map<uint64_t, IncludeFilePtr>	m_codeFileMap;
};

} // namespace Parser
LN_NAMESPACE_END

