
#include "../Internal.h"
#include <Lumino/Base/StdStringHelper.h>
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
		return m_pos == m_tokenList->getCount();
	}

	const fl::Token* GetToken() const
	{
		return m_tokenList->getAt(m_pos);
	}

	TokenPosition next(bool eatSpace = true) const
	{
		TokenPosition next(m_tokenList, m_pos + 1);
		return next.EatSpace();
	}

	TokenPosition EatSpace() const
	{
		int i = m_pos;
		for (; i < m_tokenList->getCount(); i++)
		{
			const fl::Token* t = m_tokenList->getAt(i);
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
		for (; i < m_tokenList->getCount(); i++)
		{
			if (pred(m_tokenList->getAt(i)))
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
void ShaderAnalyzer::analyzeLNFX(const char* code, int len)
{
	m_code = code;
	m_codeLength = len;

	fl::InputFile file(PathNameA{}, code, len);
	fl::CppLexer lexer;
	fl::DiagnosticsItemSet diag(PathNameA{});
	file.setDiag(&diag);	// TODO: InputFile の中でつくるべきな気がする
	lexer.Tokenize(&file);
	parseSimpleShaderMacros(file.GetTokenList());
}

//------------------------------------------------------------------------------
void ShaderAnalyzer::parseSimpleShaderMacros(const fl::TokenList* tokenList)
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

		pos = keyword.next();	// to '('
		pos = pos.next();	// to FuncName

		if (keyword.GetToken()->EqualString("technique", 9))
		{
			lastTechniqueKWLoc = keyword.GetToken()->GetBeginLoc();
		}
		else if (keyword.GetToken()->EqualString("LN_SURFACE_VS", 13))
		{
			m_wrapedShaderFuncList.add({ lastTechniqueKWLoc, "", ShaderCodeType::SurfaceVertexShader });
		}
		else if (keyword.GetToken()->EqualString("LN_SURFACE_PS", 13))
		{
			m_wrapedShaderFuncList.add({ lastTechniqueKWLoc, pos.GetToken()->getString().c_str(), ShaderCodeType::SurfacePixelShader });
		}

		pos = keyword.next();
	}
}

//------------------------------------------------------------------------------
std::vector<char> ShaderAnalyzer::makeHLSLCode() const
{
	std::stringstream sb;
	// Shader common header.
	{
		static const unsigned char EffectHeader_Data[] =
		{
#include "Resource/EffectHeaderDX9HLSL.fxh.h"
		};
		static const size_t EffectHeader_Data_Len = LN_ARRAY_SIZE_OF(EffectHeader_Data);
		sb << (EffectHeader_Data, EffectHeader_Data_Len);
	}
	sb << ("#line 5");
	sb << ("\n");
	//newCode.Append(m_code, m_codeLength);
	//newCode.Append("\n");	// 最後には改行を入れておく。環境によっては改行がないとエラーになる。しかもエラーなのにエラー文字列が出ないこともある。

	std::string newCode = sb.str();

	std::vector<char> code(newCode.c_str(), newCode.c_str() + newCode.length());
	code.insert(code.end(), m_code, m_code + m_codeLength);

	// TODO: StringBuilderA Insert, AppendFormat, AppendLine

	int insetedLength = newCode.length();

	for (auto& info : m_wrapedShaderFuncList)
	{
		if (info.type == ShaderCodeType::SurfacePixelShader)
		{
			std::stringstream sb;
			sb << (StdStringHelper::cat<std::string>("LN_PS_OUTPUT_SURFACE LN_SurfacePS_", info.funcName, "(LN_PS_INPUT_SURFACE i){"));
			sb << ("LN_SURFACE_INPUT ui;");
			sb << ("ui.TexCoord  = i.TexCoord;");
			sb << ("ui.TexCoord1 = i.TexCoord1;");
			sb << ("ui.TexCoord2 = i.TexCoord2;");
			sb << ("ui.TexCoord3 = i.TexCoord;");
			sb << ("ui.Color     = i.Color;");
			sb << (StdStringHelper::cat<std::string>("LN_SURFACE_OUTPUT uo = ", info.funcName, "(ui);"));
			sb << ("LN_PS_OUTPUT_SURFACE o;");
			sb << ("o.Color = uo.Albedo;");
			sb << ("return o;");
			sb << ("}");
			std::string str = sb.str();
			code.insert(code.begin() + insetedLength + info.lastTechniqueKWLoc, str.c_str(), str.c_str() + str.length());
			insetedLength += str.length();
		}
	}


	code.push_back('\n');	// 最後には改行を入れておく。環境によっては改行がないとエラーになる。しかもエラーなのにエラー文字列が出ないこともある。


	return code;
}

} // namespace detail
LN_NAMESPACE_END
