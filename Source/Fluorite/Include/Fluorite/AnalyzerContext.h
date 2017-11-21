
#pragma once
#include "Token.h"

namespace fl {
class DiagnosticsItemSet;
class DiagnosticsManager;
class AbstractLexer;
class AnalyzerContext;

enum class InputFileCategory
{
	CompileUnit,
	Header,
};

class TranslationUnit
	: public Object
{
	List<Token>	m_tokens;
};

/**
	@brief	
*/
class InputFile
	: public Object
{
public:
	InputFile(const std::string& filePath);
	InputFile(const std::string& filePath, const char* code, int length);
	~InputFile() = default;

	void Lex();

	Language GetLanguage() const { return m_lang; }
	InputFileCategory GetCategory() const { return m_category; }
	const std::string& GetRelativeFilePath() const { return m_filePath; }
	DiagnosticsItemSet* getDiag() const { return m_diag; }
	TokenList* GetTokenList() { return &m_tokenList; }

LN_INTERNAL_ACCESS:
	void setOwnerContext(AnalyzerContext* context) { m_context = context; }
	ByteBuffer* GetCodeBuffer();
	TokenList* GetTokenListInternal() { return &m_tokenList; }
	void setDiag(DiagnosticsItemSet* diag) { m_diag = diag; }
	SourceToken* addSourceToken(TokenGroup group, const char* bufBegin, const char* bufEnd, int tokenType);

private:
	void ReadFile();

	AnalyzerContext*	m_context;
	Language			m_lang;
	InputFileCategory	m_category;
	std::string			m_filePath;
	ByteBuffer			m_code;
	bool				m_codeRead;
	TokenList			m_tokenList;
	DiagnosticsItemSet*	m_diag;

	Ref<TranslationUnit>	m_translationUnit;	// .h などの場合は null
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
	InputFile* RegisterInputFile(const std::string& filePath);

	/** メモリ上のデータをファイルである可能に登録する */
	InputFile* RegisterInputMemoryCode(const std::string& filePath, const char* code, int length = -1);

	void RemoveAllInputFile();

	void Analyze();

	void LexAll();
	void LexFile(InputFile* file);
	
	void PreprocessAll();
	void PreprocessFile(InputFile* file);

LN_INTERNAL_ACCESS:
	Ref<AbstractLexer> CreateLexer(InputFile* file);


private:
	void ResetFileDiagnostics(InputFile* file);

	List<Ref<InputFile>>		m_inputFileList;

	Ref<DiagnosticsManager>	m_diagnosticsManager;
};

} // namespace fl
