
#pragma once
#include "Token.h"

namespace fl {
class DiagnosticsItemSet;
class DiagnosticsManager;
class AbstractLexer;

enum class InputFileCategory
{
	CompileUnit,
	Header,
};

class TokenStore
{
public:
	TokenStore();
	~TokenStore();	

	void Reserve(int count);
	Token* CreateToken();

private:
	List<Token*>	m_tokenStore;
};

/**
	@brief	
*/
class InputFile
	: public Object
{
public:
	InputFile(const PathNameA& filePath);
	InputFile(const PathNameA& filePath, const char* code, int length);
	~InputFile() = default;

	Language GetLanguage() const { return m_lang; }
	InputFileCategory GetCategory() const { return m_category; }
	const PathNameA& GetRelativeFilePath() const { return m_filePath; }
	DiagnosticsItemSet* GetDiag() const { return m_diag; }
	const TokenList* GetTokenList() const { return &m_tokenList; }

LN_INTERNAL_ACCESS:
	ByteBuffer* GetCodeBuffer();
	TokenList* GetTokenListInternal() { return &m_tokenList; }
	void SetDiag(DiagnosticsItemSet* diag) { m_diag = diag; }
	Token* CreateToken();

private:
	void ReadFile();

	Language			m_lang;
	InputFileCategory	m_category;
	PathNameA			m_filePath;
	ByteBuffer			m_code;
	bool				m_codeRead;
	TokenStore			m_tokenStore;
	TokenList			m_tokenList;
	DiagnosticsItemSet*	m_diag;
};

/**
	@brief	構造解析のルートオブジェクト
*/
class AnalyzerContext
{
public:
	AnalyzerContext();
	virtual ~AnalyzerContext();

	/** ソースファイルを登録する */
	InputFile* RegisterInputFile(const PathNameA& filePath);

	/** メモリ上のデータをファイルである可能に登録する */
	InputFile* RegisterInputMemoryCode(const PathNameA& filePath, const char* code, int length = -1);

	void RemoveAllInputFile();

	void Analyze();

	void LexAll();
	void LexFile(InputFile* file);
	
	void PreprocessAll();
	void PreprocessFile(InputFile* file);

private:
	void ResetFileDiagnostics(InputFile* file);
	RefPtr<AbstractLexer> CreateLexer(InputFile* file);

	List<RefPtr<InputFile>>		m_inputFileList;

	RefPtr<DiagnosticsManager>	m_diagnosticsManager;
};

} // namespace fl
