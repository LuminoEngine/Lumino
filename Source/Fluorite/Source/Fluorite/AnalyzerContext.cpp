
#include "Internal.h"
#include <Lumino/IO/FileSystem.h>
#include <Fluorite/Diagnostics.h>
#include <Fluorite/AnalyzerContext.h>
#include "Lexer/CppLexer.h"

namespace fl
{
//
////==============================================================================
//// TokenStore
////==============================================================================
//
////------------------------------------------------------------------------------
//TokenStore::TokenStore()
//{
//}
//
////------------------------------------------------------------------------------
//TokenStore::~TokenStore()
//{
//	for (SourceToken* t : m_tokenStore)
//	{
//		delete t;
//	}
//}
//
////------------------------------------------------------------------------------
//void TokenStore::reserve(int count)
//{
//	if (LN_REQUIRE(m_tokenStore.isEmpty())) return;
//	m_tokenStore.reserve(count);
//}
//
////------------------------------------------------------------------------------
//SourceToken* TokenStore::CreateToken()
//{
//	// TODO: ちゃんとバッファをキャッシュしたい。これだと doxygen みたいにすごく重い。
//	SourceToken* t = LN_NEW SourceToken();
//	m_tokenStore.add(t);
//	return t;
//}

//==============================================================================
// InputFile
//==============================================================================

InputFile::InputFile(const std::string& filePath)
	: m_context(nullptr)
	, m_lang(Language::Cpp11)
	, m_category(InputFileCategory::CompileUnit)
	, m_filePath(filePath)
	, m_codeRead(false)
	, m_diag(nullptr)
{
}

InputFile::InputFile(const std::string& filePath, const char* code, int length)
	: m_context(nullptr)
	, m_lang(Language::Cpp11)
	, m_filePath(filePath)
	, m_code(code, (length < 0) ? strlen(code) : length)
	, m_codeRead(true)
	, m_diag(nullptr)
{
}

void InputFile::Lex()
{
	if (LN_REQUIRE(m_context)) return;
	auto lexer = m_context->CreateLexer(this);
	lexer->Tokenize(this);
}

void InputFile::ReadFile()
{
	if (!m_codeRead)
	{
		// TODO: 文字コード変換
		m_code = FileSystem::readAllBytes(String::fromCString(m_filePath.c_str()));
		m_codeRead = true;
	}
}

ByteBuffer* InputFile::GetCodeBuffer()
{
	ReadFile();
	return &m_code;
}

SourceToken* InputFile::addSourceToken(TokenGroup group, const char* begin, const char* end, int tokenType)
{
	// 初回、最大のパターンで容量確保
	if (m_tokenList.isEmpty())
	{
		m_tokenList.reserve(m_code.getSize());
	}

	const char* codeBegin = (const char*)m_code.getConstData();
	m_tokenList.add(SourceToken(this, group, begin - codeBegin, end - codeBegin, tokenType));
	return &m_tokenList.getLast();
}

//==============================================================================
// AnalyzerContext
//==============================================================================

AnalyzerContext::AnalyzerContext()
{
	m_diagnosticsManager = Ref<DiagnosticsManager>::makeRef();
}

AnalyzerContext::~AnalyzerContext()
{
}

InputFile* AnalyzerContext::RegisterInputFile(const std::string& filePath)
{
	auto ptr = Ref<InputFile>::makeRef(filePath);
	m_inputFileList.add(ptr);
	ptr->setOwnerContext(this);
	return ptr;
}

InputFile* AnalyzerContext::RegisterInputMemoryCode(const std::string& filePath, const char* code, int length)
{
	if (LN_REQUIRE(code != nullptr)) return nullptr;
	auto ptr = Ref<InputFile>::makeRef(filePath, code, length);
	m_inputFileList.add(ptr);
	return ptr;
}

void AnalyzerContext::RemoveAllInputFile()
{
	m_inputFileList.clear();
}

void AnalyzerContext::LexAll()
{
	for (InputFile* file : m_inputFileList)
	{
		LexFile(file);
	}
}

void AnalyzerContext::LexFile(InputFile* file)
{
	if (LN_REQUIRE(file != nullptr)) return;
	ResetFileDiagnostics(file);
	auto lexer = CreateLexer(file);
	lexer->Tokenize(file);
}

void AnalyzerContext::PreprocessAll()
{
	for (InputFile* file : m_inputFileList)
	{
		PreprocessFile(file);
	}
}

void AnalyzerContext::PreprocessFile(InputFile* file)
{
	if (LN_REQUIRE(file != nullptr)) return;
	if (LN_REQUIRE(file->GetCategory() != InputFileCategory::CompileUnit)) return;

	LN_NOTIMPLEMENTED();
}

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


