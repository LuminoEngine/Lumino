
#pragma once
#include "../Base/String.h"

LN_NAMESPACE_BEGIN
namespace tr {

/**
	@brief	不正な XML フォーマットが入力された場合にスローされる例外です。
*/
class XmlException
	: public Exception
{
public:
	LN_EXCEPTION_BASIC_CONSTRUCTOR_DECLARE(XmlException);

public:
	// override Exception
	virtual Exception* copy() const { return LN_NEW XmlException(*this); }
};

namespace detail
{
enum XmlErrorCode
{
	ParseError_NoError = 0,						///< エラーは発生していない
	ParseError_TagMismatch,


	ParseError_CommentDoubleHyphen,		///< コメント内に隣接する -- が見つかった

	ParseError_ElementNameNotFount,			///< 要素名が見つからなかった
	ParseError_ElementInvalidEmptyTagEnd,		///< 単一要素の終端が /> ではない

	ParseError_AttributeEqualNotFount,	///< 属性の = が見つからなかった
	ParseError_AttributeQuoteNotFount,	///< 属性の値が引用符で囲まれていない

	ParseError_InvalidEOF,		///< 予期せぬ EOF
};

class XmlError
{
public:
	XmlErrorCode errorCode = ParseError_NoError;
	String filePath;
	int line = 0;
	int col = 0;
	String message;

	XmlError() {}
	~XmlError() {}

	void AddError(XmlErrorCode errorCode_, int line_, int col_, const String& message_)
	{
		errorCode = errorCode_;
		line = line_;
		col = col_;
		message = String::format(_T("{3}({0}, {1}): {2}"), line, col, message_, filePath);
	}
	void AddError(XmlErrorCode errorCode_, int line_, int col_)	// TODO: 削除予定。エラーメッセージはちゃんとつけてあげよう。
	{
		AddError(errorCode_, line_, col_, String());
	}

	bool HasError() const { return errorCode != ParseError_NoError; }
};

} // namespace detail
} // namespace tr
LN_NAMESPACE_END
