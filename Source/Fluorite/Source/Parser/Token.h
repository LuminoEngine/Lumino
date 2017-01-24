
#pragma once
#include "Common.h"
//#include <memory>
//#include "../../../external/Lumino.Core/include/Lumino/Base/ByteBuffer.h"
//#include "../../../external/Lumino.Core/include/Lumino/Base/Array.h"
//#include "../../../external/Lumino.Core/include/Lumino/Base/String.h"
//#include "../../../external/Lumino.Core/include/Lumino/Base/StringTraits.h"

LN_NAMESPACE_BEGIN
namespace parser
{
	
/**
	@brief	
*/
class Token
{
public:
	static const Token EofToken;


private:
	Token(CommonTokenType commonType/*, const TokenChar* begin, const TokenChar* end*/)
	{
		m_commonType = commonType;
	}
	//Token(CommonTokenType commonType, const TokenChar* begin, const TokenChar* end, int langTokenType)
	//	: Token(commonType, begin, end)
	//{
	//	m_langTokenType = langTokenType;
	//}

public:
	Token() = default;
	Token(const Token& src) = default;
	Token& operator = (const Token& src) = default;
	~Token() = default;

	CommonTokenType GetCommonType() const { return m_commonType; }
	int GetLangTokenType() const { return m_langTokenType; }
	const TokenChar* GetBegin() const;	// TODO: inline にしたい
	const TokenChar* GetEnd() const;
	int GetLength() const { return m_locEnd - m_locBegin; }

	bool IsSpaceOrComment() const { return m_commonType == CommonTokenType::SpaceSequence || m_commonType == CommonTokenType::Comment; }
	bool IsEof() const { return m_commonType == CommonTokenType::Eof; }

	// 文字列が一致するか
	bool EqualString(const char* str, int len) const
	{
		if (GetLength() != len) return false;
		return StringTraits::StrNCmp(GetBegin(), str, len) == 0;	// TODO: Case
	}

	// 文字が一致するか
	bool EqualChar(char ch) const
	{
		if (GetLength() != 1) return false;
		return *GetBegin() == ch;	// TODO: Case
	}

	GenericString<TokenChar> ToString() const { return GenericString<TokenChar>(GetBegin(), GetLength()); }

	TokenStringRef GetStringRef() const { return TokenStringRef(GetBegin(), GetLength()); }

	// トークンがマクロの場合、そのマクロの実体への参照
	// 実際に使うとしても、マクロ定義がネストしている場合は NULL。
	// マクロを展開したとき、その中にあるマクロの置換は直近の定義に従う。・・・って言うと、1つずつ検索する方法に統一したほうが良いか・・・？
	void SetMacroEntity(MacroDefine* macro) { m_macroEntity = macro; }
	MacroDefine* GetMacroEntity() const { return m_macroEntity; }

	void SetValid(bool valid) { m_valid = valid; }
	bool IsValid() const { return m_valid; }

	void SetFirstLineNumber(int lineNumber) { m_firstLineNumber = lineNumber; }
	void SetFirstColumn(int column) { m_firstColumn = column; }
	void SetLastLineNumber(int lineNumber) { m_lastLineNumber = lineNumber; }
	void SetLastColumn(int column) { m_lastColumn = column; }

	int GetFirstLineNumber() const { return m_firstLineNumber; }
	int GetFirstColumn() const { return m_firstColumn; }
	int GetLastLineNumber() const { return m_lastLineNumber; }
	int GetLastColumn() const { return m_lastColumn; }

private:
	friend class TokenBuffer;
	CommonTokenType		m_commonType = CommonTokenType::Unknown;
	int					m_langTokenType = 0;
	int					m_firstLineNumber = -1;		// 0スタート
	int					m_firstColumn = -1;			// 0スタート
	int					m_lastLineNumber = -1;		// 0スタート
	int					m_lastColumn = -1;			// 0スタート


	TokenBuffer*		m_ownerBuffer = nullptr;
	DataLocation		m_locBegin = 0;				// m_ownerBuffer 内の文字列のある先頭インデックス
	DataLocation		m_locEnd = 0;				// m_ownerBuffer 内の文字列のある終端インデックス

	MacroDefine*		m_macroEntity = nullptr;

	bool				m_valid = true;
};

#if 0
// 基本的なトークン種別 (言語間共通)
enum TokenType
{
	TokenType_Unknown = 0,
	TokenType_SpaceSequence,
	TokenType_NewLine,
	TokenType_Identifier,
	TokenType_Keyword,
	TokenType_NumericLiteral,

	/*
	 *	C/C++ の「string-literal:」にあたり、L のようなプレフィックスも含む点に注意。
	 *	文法上、プレフィックスとクォーテーションの間に空白を含めることはできない。
	 *	C# の @ も含まれる。
	 */
	 TokenType_StringLiteral,
	 TokenType_CharLiteral,			///< RPNParser で数値として評価できるため StringLiteral とは分けている
	//TokenType_CharOrStringLiteral,

	TokenType_Operator,
	TokenType_Comment,				///< 行末\ がある場合は含める
	TokenType_EscNewLine,			///< 行末 \ (TokenType_NewLine とは区別する。"\n" = NewLine, "\\\n" = EscNewLine)
	TokenType_MBSSequence,			///< マルチバイト文字並び
	TokenType_EOF,					///< EOF
};

template<typename TChar>
class Token
{
public:
	typename typedef GenericString<TChar> StringT;

public:
	Token()
	{
		Init();
	}

	Token(TokenType type, const TChar* pBegin, const TChar* pEnd)
	{
		Init();
		m_type = type;
		m_begin = pBegin;
		m_end = pEnd;
	}

	Token(TokenType type, int lnagTokenType, const TChar* pBegin, const TChar* pEnd)
		: m_type(type)
		, m_langTokenType(lnagTokenType)
		, m_begin(pBegin)
		, m_end(pEnd)
	{
		Init();
		m_type = type;
		m_langTokenType = lnagTokenType;
		m_begin = pBegin;
		m_end = pEnd;
	}

	~Token()
	{
		//LN_SAFE_DELETE_ARRAY(m_dynamicBuffer);
	}

	void Init()
	{
		m_type = TokenType_Unknown;
		m_langTokenType = 0;
		m_begin = NULL;
		m_end = NULL;
		m_beginStringValue = NULL;
		m_endStringValue = NULL;
		m_staticBuffer[0] = 0x00;
		m_dynamicBuffer = NULL;
	}


	Token(const Token& src) { Copy(src); }
	Token& operator = (const Token& src) { Copy(src); return *this; }


	void Copy(const Token& src)
	{
		m_type				= src.m_type;
		m_langTokenType		= src.m_langTokenType;
		m_begin				= src.m_begin;
		m_end				= src.m_end;
		m_beginStringValue	= src.m_beginStringValue;
		m_endStringValue	= src.m_endStringValue;
		m_dynamicBuffer		= src.m_dynamicBuffer;

		// src が static バッファを使っている場合はコピーした上でこちらのポインタも付け替える
		if (src.m_begin == src.m_staticBuffer)
		{
			memcpy_s(m_staticBuffer, sizeof(m_staticBuffer), src.m_staticBuffer, sizeof(m_staticBuffer));
			m_begin = m_staticBuffer;
			m_end = m_begin + src.GetLength();
			m_beginStringValue = m_begin + (src.m_beginStringValue - src.m_begin);
			m_endStringValue = m_end - (src.m_end - src.m_endStringValue);
		}
	}

public:
	TokenType		GetTokenType() const { return m_type; }
	int				GetLangTokenType() const { return m_langTokenType; }
	const TChar*	GetTokenBegin() const { return m_begin; }
	const TChar*	GetTokenEnd() const { return m_end; }
	int				GetLength() const { return m_end - m_begin; }

	// ※ちょっと問題あり。プリプロセッサでは \n は空白扱いしてはダメ
	bool IsGenericSpace() const
	{
		return (
			m_type == TokenType_SpaceSequence ||
			m_type == TokenType_Comment ||
			m_type == TokenType_EscNewLine);
		/* TokenType_EscNewLine も空白である。
		* # \
		* include "hoge.h"
		* はコンパイル可能。
		*/
	}

	bool IsGenericToken() const
	{
		return !IsGenericSpace();
	}

	bool IsEOF() const
	{
		return m_type == TokenType_EOF;
	}

	bool IsLineEnd() const
	{
		return (
			m_type == TokenType_NewLine ||
			m_type == TokenType_EOF);
	}

	// 1文字に一致するか。トークン自体の長さも1文字である。
	bool EqualChar(TChar ch) const
	{
		if (m_begin[0] != ch) return false;
		if (GetLength() != 1) return false;
		return true;
	}

	// 文字列が一致するか (strncmp のような部分一致ではない。長さが違えばその時点で false)
	bool EqualString(const TChar* str, int len) const
	{
		if (GetLength() != len) return false;
		return StringUtils::StrNCmp(m_begin, str, len) == 0;
	}

	void SetStringValue(const TChar* beginStringValue, const TChar* endStringValue)
	{
		m_beginStringValue = beginStringValue;
		m_endStringValue = endStringValue;
	}

	StringT GetStringValue() const { return StringT(m_beginStringValue, m_endStringValue - m_beginStringValue); }

	void CloneTokenStrings()
	{
		if (m_begin == m_staticBuffer || m_dynamicBuffer != NULL) { return; }	// すでに Clone 済み

		TChar* newBegin;
		size_t len = GetLength();
		// 32 文字に収まるトークン長さであれば static にコピーし、それを参照する
		if (len <= StaticStringLength)
		{
			memcpy_s(m_staticBuffer, sizeof(m_staticBuffer), m_begin, len);
			newBegin = m_staticBuffer;
		}
		// 収まらなければ new
		else
		{
			if (m_dynamicBuffer == NULL) {
				m_dynamicBuffer = std::tr1::shared_ptr<TChar>(new TChar[len]);
				memcpy_s(m_dynamicBuffer.get(), sizeof(TChar) * len, m_begin, sizeof(TChar) * len);
			}
			newBegin = m_dynamicBuffer.get();
		}

		// トークン参照範囲を再計算する
		m_beginStringValue = newBegin + (m_beginStringValue - m_begin);
		m_endStringValue = (newBegin + len) - (m_end - m_endStringValue);
		m_begin = newBegin;
		m_end = newBegin + len;
	}

private:
	TokenType		m_type;
	int				m_langTokenType;
	const TChar*	m_begin;
	const TChar*	m_end;
	const TChar*	m_beginStringValue;
	const TChar*	m_endStringValue;


	static const int StaticStringLength = 32;
	TChar	m_staticBuffer[StaticStringLength];		// 終端 \0 ではない
	//TChar*	m_dynamicBuffer;
	std::tr1::shared_ptr<TChar> m_dynamicBuffer;

	/*
		他にも最終的に必要になりそうな情報は…
		・意味解析レベルでの種別(クラス名？関数名？マクロ？)
		・宣言元ファイル名と行番号
		・完全修飾名
		・無効領域のトークンであるか
		…と思ったけど、意味解析レベルでは他の Varable クラスとか作って、そのメンバとして Token 持たせる方がいいと思う。
		そうすると必要になるのは無効トークンかどうかくらい？
		・マクロによって展開されたトークンであるか→処理後、畳むために必用
		・親の意味オブジェクト

		なお、整形を行う場合は完全に元に戻すのは不可能。
		もう中間言語からのデコンパイルと考えた方がいいかもしれない。
		ただ、プリプロ文は残しておきたい気もする…。
	*/


};
#endif

} // namespace Parser
LN_NAMESPACE_END

