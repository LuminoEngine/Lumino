
#include "../Internal.h"
#include <Fluorite/Diagnostics.h>
#include <Fluorite/AnalyzerContext.h>
#include "../../../Fluorite/Source/Fluorite/Lexer/CppLexer.h"
#include "ShaderAnalyzer.h"

LN_NAMESPACE_BEGIN
namespace detail {

class TokenPosition
{
public:
	TokenPosition(const fl::TokenList* tokenList, int	pos)
		: m_tokenList(tokenList)
		, m_pos(pos)
	{}

	TokenPosition& operator=(const TokenPosition& rhs)
	{
		m_tokenList = rhs.m_tokenList;
		m_pos = rhs.m_pos;
		return *this;
	}

	const fl::TokenList*	m_tokenList;
	int						m_pos;

	int GetPos() const
	{
		return m_pos;
	}

	bool IsEof() const
	{
		return m_pos == m_tokenList->GetCount();
	}

	const fl::Token* GetToken() const
	{
		return m_tokenList->GetAt(m_pos);
	}

	TokenPosition Next(bool eatSpace = true) const
	{
		TokenPosition next(m_tokenList, m_pos + 1);
		return next.EatSpace();
	}

	TokenPosition EatSpace() const
	{
		int i = m_pos;
		for (; i < m_tokenList->GetCount(); i++)
		{
			const fl::Token* t = m_tokenList->GetAt(i);
			if (t->GetTokenGroup() == fl::TokenGroup::SpaceSequence ||
				t->GetTokenGroup() == fl::TokenGroup::NewLine ||
				t->GetTokenGroup() == fl::TokenGroup::Comment)
			{
			}
			else
			{
				break;
			}
		}
		return TokenPosition(m_tokenList, i);
	}

	TokenPosition FindNext(fl::TokenGroup group, const char* str, int len) const
	{
		return FindNext([group, str, len](const fl::Token* t)
		{
			return t->EqualGroupAndString(group, str, len);
		});
	}

	template<typename TPred>
	TokenPosition FindNext(TPred pred) const
	{
		int i = m_pos;
		for (; i < m_tokenList->GetCount(); i++)
		{
			if (pred(m_tokenList->GetAt(i)))
			{
				break;
			}
		}
		return TokenPosition(m_tokenList, i);
	}
};

//==============================================================================
// ShaderAnalyzer
//==============================================================================

//------------------------------------------------------------------------------
void ShaderAnalyzer::AnalyzeLNFX(const char* code, int len)
{
	m_code = code;
	m_codeLength = len;

	fl::InputFile file(PathNameA{}, code, len);
	fl::CppLexer lexer;
	fl::DiagnosticsItemSet diag(PathNameA{});
	file.SetDiag(&diag);	// TODO: InputFile の中でつくるべきな気がする
	lexer.Tokenize(&file);
	ParseSimpleShaderMacros(file.GetTokenList());
}

//------------------------------------------------------------------------------
void ShaderAnalyzer::ParseSimpleShaderMacros(const fl::TokenList* tokenList)
{
	TokenPosition pos(tokenList, 0);
	int lastTechniqueKWLoc = 0;

	while (true)
	{
		TokenPosition keyword = pos.FindNext([](const fl::Token* t)
		{
			return
				t->EqualGroupAndString(fl::TokenGroup::Identifier, "technique", 9) ||
				t->EqualGroupAndString(fl::TokenGroup::Identifier, "LN_SURFACE_VS", 13) ||
				t->EqualGroupAndString(fl::TokenGroup::Identifier, "LN_SURFACE_PS", 13);
		});
		if (keyword.IsEof())
		{
			return;
		}

		pos = keyword.Next();	// to '('
		pos = pos.Next();	// to FuncName

		if (keyword.GetToken()->EqualString("technique", 9))
		{
			lastTechniqueKWLoc = keyword.GetToken()->GetBeginLoc();
		}
		else if (keyword.GetToken()->EqualString("LN_SURFACE_VS", 13))
		{
			m_wrapedShaderFuncList.Add({ lastTechniqueKWLoc, "", ShaderCodeType::SurfaceVertexShader });
		}
		else if (keyword.GetToken()->EqualString("LN_SURFACE_PS", 13))
		{
			m_wrapedShaderFuncList.Add({ lastTechniqueKWLoc, pos.GetToken()->GetString(), ShaderCodeType::SurfacePixelShader });
		}

		pos = keyword.Next();
	}
}

//------------------------------------------------------------------------------
std::vector<char> ShaderAnalyzer::MakeHLSLCode() const
{
	StringBuilderA newCode;
	// Shader common header.
	{
		static const unsigned char EffectHeader_Data[] =
		{
#include "Resource/EffectHeaderDX9HLSL.fxh.h"
		};
		static const size_t EffectHeader_Data_Len = LN_ARRAY_SIZE_OF(EffectHeader_Data);
		newCode.Append(EffectHeader_Data, EffectHeader_Data_Len);
	}
	newCode.Append("#line 5");
	newCode.Append(StringA::GetNewLine().c_str());
	//newCode.Append(m_code, m_codeLength);
	//newCode.Append("\n");	// 最後には改行を入れておく。環境によっては改行がないとエラーになる。しかもエラーなのにエラー文字列が出ないこともある。



	std::vector<char> code(newCode.c_str(), newCode.c_str() + newCode.GetLength());
	code.insert(code.end(), m_code, m_code + m_codeLength);

	// TODO: StringBuilderA Insert, AppendFormat, AppendLine

	int insetedLength = newCode.GetLength();

	for (auto& info : m_wrapedShaderFuncList)
	{
		if (info.type == ShaderCodeType::SurfacePixelShader)
		{
			StringBuilderA sb;
			sb.Append(StringA::Format("LN_PS_OUTPUT_SURFACE LN_SurfacePS_{0}(LN_PS_INPUT_SURFACE i){{", info.funcName));
			sb.Append("LN_SURFACE_INPUT ui;");
			sb.Append("ui.TexCoord  = i.TexCoord;");
			sb.Append("ui.TexCoord1 = i.TexCoord1;");
			sb.Append("ui.TexCoord2 = i.TexCoord2;");
			sb.Append("ui.TexCoord3 = i.TexCoord;");
			sb.Append("ui.Color     = i.Color;");
			sb.Append(StringA::Format("LN_SURFACE_OUTPUT uo = {0}(ui);", info.funcName));
			sb.Append("LN_PS_OUTPUT_SURFACE o;");
			sb.Append("o.Color = i.Color;");
			sb.Append("return o;");
			sb.Append("}");

			code.insert(code.begin() + insetedLength + info.lastTechniqueKWLoc, sb.c_str(), sb.c_str() + sb.GetLength());
			insetedLength += sb.GetLength();
		}
	}


	code.push_back('\n');	// 最後には改行を入れておく。環境によっては改行がないとエラーになる。しかもエラーなのにエラー文字列が出ないこともある。


	return code;
}

} // namespace detail
LN_NAMESPACE_END
