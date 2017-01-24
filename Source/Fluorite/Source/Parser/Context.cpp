
#include "Internal.h"
#include "Frontend\Cpp\CppLexer.h"
#include "DiagnosticsManager.h"
#include "UnitFile.h"
#include "Context.h"

LN_NAMESPACE_BEGIN
namespace parser
{

//=============================================================================
// Context
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Context::Clear()
{
	m_codeFileMap.clear();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ResultState Context::LookupPreprocessedIncludeFile(
	CompileUnitFile* rootCompileUnitFile,
	const TokenPathName& basePath,
	const TokenPathName& filePath,
	const Array<TokenPathName>* additionalIncludePaths,
	const MacroMapContainer& parentMacroMap,
	ReferenceTracker* referenceTracker,
	DiagnosticsItemSet* parentDiag,
	IncludeFile** outFile)
{
	assert(additionalIncludePaths != nullptr);
	//LN_CHECK_ARGS_RETURNV(additionalIncludePaths != nullptr, ResultState::Error);
	//LN_CHECK_ARGS_RETURNV(macroMap != nullptr, ResultState::Error);

	//------------------------------------------------------------
	// include ファイルを検索する
	PathName includeFilePath;
	// 絶対パスならそのまま使う
	if (filePath.IsAbsolute())
	{
		includeFilePath = filePath;
	}
	// 親コードファイルからの相対パスに存在するか？
	if (basePath.ExistsFileInDirectory(filePath))
	{
		includeFilePath = PathName(basePath, filePath);
	}
	// -I で指定されたフォルダに存在するか？
	else
	{
		for (auto& path : *additionalIncludePaths)
		{
			if (path.ExistsFileInDirectory(filePath))
			{
				includeFilePath = PathName(path, filePath);
				break;
			}
		}
	}

	// Error: include ファイルが見つからなかった
	if (includeFilePath.IsEmpty())
	{
		parentDiag->Report(DiagnosticsCode::Preprocessor_IncludeFileNotFount, includeFilePath.GetString());
		return ResultState::Error;
	}

	// 絶対パスにする
	includeFilePath = includeFilePath.CanonicalizePath();


	//------------------------------------------------------------

	//if (rootCompileUnitFile->includedFiles.Contains([&](IncludeFile* item){ return item->GetAbsolutePath().Equals(includeFilePath); }))
	//{

	//}




	uint64_t key = MakeCacheFileKey(includeFilePath, additionalIncludePaths, parentMacroMap.GetConst());

	auto itr = m_codeFileMap.find(key);
	if (itr != m_codeFileMap.end())
	{
		*outFile = itr->second;
		return ResultState::Success;	// TODO: CodeFile に解析結果を持たせたい
	}
	else
	{
		// ファイル内容を一度全て読み込む
		// TODO: 文字コード
		FileStreamPtr file = FileStream::Create(includeFilePath, FileOpenMode::Read);
		ByteBuffer buffer(file->GetLength(), false);
		file->Read(buffer.GetData(), buffer.GetSize());

		auto codeFile = RefPtr<IncludeFile>::MakeRef();
		codeFile->Initialize(includeFilePath);

		// トークン分割
		CppLexer lex;
		TokenListPtr tokens = lex.Tokenize(buffer, parentDiag);	// TODO: エラー
		codeFile->SetTokenList(tokens);				// トークリストは全部覚えておく (define の置換要素とエンティティ参照を取り出したら消しても良い)


		//printf("%d\n", parentMacroMap.GetConst()->IsDefined("AAA"));

		// プリプロセス
		Preprocessor preprocessor;
		LN_RESULT_CALL(preprocessor.BuildPreprocessedTokenList(this, rootCompileUnitFile, tokens, codeFile, additionalIncludePaths, parentMacroMap, referenceTracker, parentDiag));
		//codeFile->GetMacroMap()->SetFreeze(true);	// 確定。あとは変化しない

		//printf("%d\n", codeFile->GetMacroMapPtr()->GetConst()->IsDefined("BBB"));

		m_codeFileMap.insert({ key, codeFile });
		*outFile = codeFile;
		return ResultState::Success;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint64_t Context::MakeCacheFileKey(const PathName& includeFilePath, const Array<TokenPathName>* additionalIncludePaths, const MacroMap* macroMap)
{
	uint64_t key = Hash::CalcHash(includeFilePath.c_str(), includeFilePath.GetLength());
	for (auto& path : *additionalIncludePaths)
	{
		key += Hash::CalcHash(path.c_str(), path.GetLength());
	}
	key += macroMap->GetHashCode();
	return key;
}

} // namespace Parser
LN_NAMESPACE_END

