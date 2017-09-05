
#include "Internal.h"
#include <Lumino/IO/FileSystem.h>
#include <Fluorite/Diagnostics.h>
#include <Fluorite/AnalyzerContext.h>
#include "Lexer/CppLexer.h"

namespace fl
{

//==============================================================================
// TokenStore
//==============================================================================

//------------------------------------------------------------------------------
TokenStore::TokenStore()
{
}

//------------------------------------------------------------------------------
TokenStore::~TokenStore()
{
	for (Token* t : m_tokenStore)
	{
		delete t;
	}
}

//------------------------------------------------------------------------------
void TokenStore::reserve(int count)
{
	if (LN_CHECK_STATE(m_tokenStore.isEmpty())) return;
	m_tokenStore.reserve(count);
}

//------------------------------------------------------------------------------
Token* TokenStore::CreateToken()
{
	// TODO: ちゃんとバッファをキャッシュしたい。これだと doxygen みたいにすごく重い。
	Token* t = LN_NEW Token();
	m_tokenStore.add(t);
	return t;
}

//==============================================================================
// InputFile
//==============================================================================

//------------------------------------------------------------------------------
InputFile::InputFile(const PathNameA& filePath)
	: m_lang(Language::Cpp11)
	, m_category(InputFileCategory::CompileUnit)
	, m_filePath(filePath)
	, m_codeRead(false)
	, m_diag(nullptr)
{
}

//------------------------------------------------------------------------------
InputFile::InputFile(const PathNameA& filePath, const char* code, int length)
	: m_lang(Language::Cpp11)
	, m_filePath(filePath)
	, m_code(code, (length < 0) ? strlen(code) : length)
	, m_codeRead(true)
	, m_diag(nullptr)
{
}

//------------------------------------------------------------------------------
void InputFile::ReadFile()
{
	if (!m_codeRead)
	{
		// TODO: 文字コード変換
		m_code = FileSystem::readAllBytes(String::fromCString(m_filePath.c_str()));
		m_codeRead = true;
	}
}

//------------------------------------------------------------------------------
ByteBuffer* InputFile::GetCodeBuffer()
{
	ReadFile();
	return &m_code;
}

//------------------------------------------------------------------------------
Token* InputFile::CreateToken()
{
	// 初回、最大のパターンで容量確保
	if (m_tokenList.isEmpty())
	{
		m_tokenStore.reserve(m_code.getSize());
	}

	Token* t = m_tokenStore.CreateToken();
	m_tokenList.add(t);
	return t;
}

//==============================================================================
// AnalyzerContext
//==============================================================================

//------------------------------------------------------------------------------
AnalyzerContext::AnalyzerContext()
{
	m_diagnosticsManager = Ref<DiagnosticsManager>::makeRef();
}

//------------------------------------------------------------------------------
AnalyzerContext::~AnalyzerContext()
{
}

//------------------------------------------------------------------------------
InputFile* AnalyzerContext::RegisterInputFile(const PathNameA& filePath)
{
	auto ptr = Ref<InputFile>::makeRef(filePath);
	m_inputFileList.add(ptr);
	return ptr;
}

//------------------------------------------------------------------------------
InputFile* AnalyzerContext::RegisterInputMemoryCode(const PathNameA& filePath, const char* code, int length)
{
	if (LN_CHECK_ARG(code != nullptr)) return nullptr;
	auto ptr = Ref<InputFile>::makeRef(filePath, code, length);
	m_inputFileList.add(ptr);
	return ptr;
}

//------------------------------------------------------------------------------
void AnalyzerContext::RemoveAllInputFile()
{
	m_inputFileList.clear();
}

//------------------------------------------------------------------------------
void AnalyzerContext::LexAll()
{
	for (InputFile* file : m_inputFileList)
	{
		LexFile(file);
	}
}

//------------------------------------------------------------------------------
void AnalyzerContext::LexFile(InputFile* file)
{
	if (LN_CHECK_ARG(file != nullptr)) return;
	ResetFileDiagnostics(file);
	auto lexer = CreateLexer(file);
	lexer->Tokenize(file);
}

//------------------------------------------------------------------------------
void AnalyzerContext::PreprocessAll()
{
	for (InputFile* file : m_inputFileList)
	{
		PreprocessFile(file);
	}
}

//------------------------------------------------------------------------------
void AnalyzerContext::PreprocessFile(InputFile* file)
{
	if (LN_CHECK_ARG(file != nullptr)) return;
	if (LN_CHECK_ARG(file->GetCategory() != InputFileCategory::CompileUnit)) return;

	LN_NOTIMPLEMENTED();
}

//------------------------------------------------------------------------------
void AnalyzerContext::ResetFileDiagnostics(InputFile* file)
{
	if (file->getDiag() != nullptr)
	{
		file->getDiag()->clearItems();
	}
	else
	{
		file->setDiag(m_diagnosticsManager->CreateItemSet(file->GetRelativeFilePath()));
	}
}

//------------------------------------------------------------------------------
Ref<AbstractLexer> AnalyzerContext::CreateLexer(InputFile* file)
{
	switch (file->GetLanguage())
	{
	case Language::Cpp11:
		return Ref<AbstractLexer>::staticCast(Ref<CppLexer>::makeRef());
	default:
		assert(0);
		break;
	}
	return nullptr;
}

} // namespace fl


